#include <gtest/gtest.h>

#include "calc/grammar.h"
#include "peg/parser.h"

using namespace calc;
using namespace peg;

TEST(CalcGrammarTest, Char) {
  auto s = "abcd";
  TextParser p(s, s + 3);
  EXPECT_FALSE((Char<'B'>::Match(p)));
  ASSERT_EQ(p.iter(), s);
  EXPECT_TRUE((Char<'a'>::Match(p)));
  ASSERT_EQ(p.iter(), s + 1);
  // if eof must return false
  p.set_iter(s + 3);
  ASSERT_TRUE(p.eof());
  EXPECT_FALSE((Char<'c'>::Match(p)));
}

TEST(CalcGrammarTest, Digit) {
  auto s = "a01234567893";
  TextParser p(s, s + 11);
  EXPECT_FALSE((Digit::Match(p)));
  ASSERT_EQ(p.iter(), s);
  p.GotoNext();
  for (int i = 0; i < 10; ++i) ASSERT_TRUE((Digit::Match(p)));
  ASSERT_EQ(p.iter(), s + 11);
  // if eof must return false
  ASSERT_TRUE(p.eof());
  EXPECT_FALSE((Digit::Match(p)));
}

TEST(CalcGrammarTest, MathExpr) {
  TextParser p;
  auto root = p.ast_root();
  EXPECT_FALSE(p.Parse<MathExpr>("10 + 1"));
  EXPECT_FALSE(p.Parse<MathExpr>("-10"));
  EXPECT_TRUE(p.Parse<MathExpr>("5"));
  EXPECT_EQ(EvalNode(root->child()), 5);
  EXPECT_TRUE(p.Parse<MathExpr>("(7)"));
  EXPECT_EQ(EvalNode(root->child()), 7);
  EXPECT_TRUE(p.Parse<MathExpr>("(7+ 3)*2 -8 /4"));
  EXPECT_EQ(EvalNode(root->child()), 18);
  EXPECT_TRUE(p.Parse<MathExpr>("(4 + 5 * (7 - 3)) - 2"));
  EXPECT_EQ(EvalNode(root->child()), 22);
  EXPECT_TRUE(p.Parse<MathExpr>("4+5+7/2"));
  EXPECT_EQ(EvalNode(root->child()), 12);
  EXPECT_TRUE(p.Parse<MathExpr>("4+5+7 - 2 * 3 - 4 / 2"));
  EXPECT_EQ(EvalNode(root->child()), 8);
}
