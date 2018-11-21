#include <gtest/gtest.h>

#include "peg/grammar.h"
#include "peg/parser.h"

using namespace peg;

template<bool b>
struct Bool {
  template<class ParserT>
  static bool Match(ParserT&) {
    return b;
  }
};

struct True: Bool<true> {};
struct False: Bool<false> {};

TEST(PegGrammarTest, Or) {
  BasicParser<char> p;
  EXPECT_TRUE((Or<True, True>::Match(p)));
  EXPECT_TRUE((Or<False, True>::Match(p)));
  EXPECT_TRUE((Or<True, False>::Match(p)));
  EXPECT_TRUE((Or<False, False, False, True>::Match(p)));
  EXPECT_FALSE((Or<False, False>::Match(p)));
  EXPECT_FALSE((Or<False, False, False, False>::Match(p)));
}
