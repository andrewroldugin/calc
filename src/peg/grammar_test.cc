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
  EXPECT_TRUE((Or<True<4>, True<5> >::Match(p)));
  EXPECT_EQ(s + 4, p.iter());
  EXPECT_TRUE((Or<False, True<> >::Match(p)));
  EXPECT_TRUE((Or<True<> , False>::Match(p)));
  EXPECT_TRUE((Or<False, False, False, True<> >::Match(p)));
  EXPECT_TRUE((Or<True<4>, True<5>, True<6> >::Match(p)));
  EXPECT_EQ(s + 8, p.iter());
  EXPECT_FALSE((Or<False, False>::Match(p)));
  EXPECT_FALSE((Or<False, False, False, False>::Match(p)));
}

TEST(PegGrammarTest, Seq) {
  auto s = "abcdefghijklmno";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Seq<>::Match(p)));
  EXPECT_FALSE((Seq<False>::Match(p)));
  EXPECT_FALSE((Seq<True<5>, False>::Match(p)));
  // iter must be restored after failed sequence
  EXPECT_EQ(s, p.iter());
  EXPECT_TRUE((Seq<True<3>, True<5>>::Match(p)));
  EXPECT_EQ(s + 8, p.iter());
}

TEST(PegGrammarTest, Star) {
  auto s = "abcdefghijklmno";
  TextParser p(s, s + std::strlen(s));
  EXPECT_TRUE((Star<False>::Match(p)));
  EXPECT_TRUE((Star<True<1>>::Match(p)));
  EXPECT_TRUE(p.eof());
}
