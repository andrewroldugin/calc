#include <cstring>
#include <gtest/gtest.h>

#include "peg/grammar.h"
#include "peg/parser.h"

using namespace peg;

template<bool b, int offset = 0>
struct Bool {
  template<class ParserT>
  static bool Match(ParserT& p) {
    if (!p.eof()) p.set_iter(p.iter() + offset); else return false;
    return b;
  }
};

// rule is true if not eof
template <int offset = 0>
struct True: Bool<true, offset> {};
struct False: Bool<false> {};

TEST(PegGrammarTest, Or) {
  auto s = "abcdefghijklmno";
  TextParser p(s, s + std::strlen(s));
  EXPECT_FALSE((Or<>::Match(p)));
  EXPECT_FALSE((Or<False>::Match(p)));
  EXPECT_TRUE((Or<True<> >::Match(p)));
  EXPECT_TRUE((Or<False, True<> >::Match(p)));
  EXPECT_TRUE((Or<True<>, False>::Match(p)));
  EXPECT_TRUE((Or<False, False, False, True<> >::Match(p)));
  EXPECT_FALSE((Or<False, False>::Match(p)));
  EXPECT_FALSE((Or<False, False, False, False>::Match(p)));

  // check order
  ASSERT_TRUE((Or<True<4>, True<5> >::Match(p)));
  ASSERT_EQ(s + 4, p.iter());
  ASSERT_TRUE((Or<True<4>, True<5>, True<6> >::Match(p)));
  ASSERT_EQ(s + 8, p.iter());
}

TEST(PegGrammarTest, Seq) {
  auto s = "abcdefghijklmno";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Seq<>::Match(p)));
  EXPECT_FALSE((Seq<False>::Match(p)));
  EXPECT_FALSE((Seq<True<5>, False>::Match(p)));
  // iter must be restored after failed sequence
  ASSERT_EQ(s, p.iter());
  ASSERT_TRUE((Seq<True<3>, True<5>>::Match(p)));
  EXPECT_EQ(s + 8, p.iter());
}

TEST(PegGrammarTest, Star) {
  auto s = "abcdefghijklmno";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Star<False>::Match(p)));
  EXPECT_TRUE((Star<True<1>>::Match(p)));
  EXPECT_TRUE(p.eof());
}

TEST(PegGrammarTest, Eof) {
  auto s = "A";
  TextParser p(s, s + 1);
  EXPECT_FALSE((Eof::Match(p)));
  p.GotoNext();
  EXPECT_TRUE((Eof::Match(p)));
}

TEST(PegGrammarTest, Store) {
  auto s = "ABC";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Store<True<3> >::Match(p)));
  auto child = p.ast_root()->child();
  ASSERT_NE(child, nullptr);
  EXPECT_EQ(child->begin(), s);
  EXPECT_EQ(child->end(), s + std::strlen(s));
}

TEST(PegGrammarTest, StoreFailed) {
  auto s = "ABC";
  TextParser p(s, s + std::strlen(s));
  EXPECT_FALSE((Store<False>::Match(p)));
  auto child = p.ast_root()->child();
  EXPECT_EQ(child, nullptr);
}

TEST(PegGrammarTest, StoreNested) {
  auto s = "ABC";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Store<Store<True<3> > >::Match(p)));
  auto child = p.ast_root()->child();
  ASSERT_NE(child, nullptr);
  EXPECT_EQ(child->next(), nullptr);
  EXPECT_EQ(child->begin(), s);
  EXPECT_EQ(child->end(), s + std::strlen(s));

  ASSERT_NE(child->child(), nullptr);
  EXPECT_EQ(child->child()->next(), nullptr);
  EXPECT_EQ(child->child()->begin(), s);
  EXPECT_EQ(child->child()->end(), s + std::strlen(s));
}

TEST(PegGrammarTest, StoreComplex) {
  auto s = "ABCDEF";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Seq<Store<True<3> >,
              Or<Store<False>, Store<True<3> > > >::Match(p)));
  auto child = p.ast_root()->child();
  ASSERT_NE(child, nullptr);
  EXPECT_EQ(child->begin(), s);
  EXPECT_EQ(child->end(), s + 3);

  ASSERT_NE(child->next(), nullptr);
  EXPECT_EQ(child->next()->begin(), s + 3);
  EXPECT_EQ(child->next()->end(), s + 6);
}

TEST(PegGrammarTest, CleanAstOnParseFail) {
  TextParser p;
  EXPECT_FALSE((p.Parse<Seq<Store<True<> >, False > >("a")));
  EXPECT_EQ(p.ast_root()->child(), nullptr);
}

TEST(PegGrammarTest, TypeMatches) {
  TextParser::Tree::TypedNode<False> node;
  EXPECT_TRUE(node.TypeMatches<False>());
  EXPECT_FALSE(node.TypeMatches<True<> >());
}
