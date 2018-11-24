#ifndef CALC_GRAMMAR_H
#define CALC_GRAMMAR_H

#include "peg/grammar.h"
#include "peg/parser.h"

namespace calc {
  using namespace peg;

  template<char C>
  struct Char {
    template<class ParserT>
    static bool Match(ParserT& p) {
      bool out = false;
      if (!p.eof()) {
        out = *p.iter() == C;
        if (out) p.GotoNext();
      }
      return out;
    }
  };

  struct Digit {
    template<class ParserT>
    static bool Match(ParserT& p) {
      bool out = false;
      if (!p.eof()) {
        out = *p.iter() >= '0' && *p.iter() <= '9';
        if (out) p.GotoNext();
      }
      return out;
    }
  };

  struct Digit;
  struct Space: Star<Or<Char<' '>, Char<'\t'> > > {};
  template <class R>
  struct Token: Seq<Space, R, Space> {};
  struct BracedExpr;
  struct Number: Digit {};
  struct Value: Token<Or<Store<Number>, BracedExpr> > {};
  template <class SignT, class OperandT>
  struct Op: Seq<OperandT, Star<Seq<SignT, OperandT> > > {};
  struct Mul: Char<'*'> {};
  struct Div: Char<'/'> {};
  struct Product: Op<Or<Store<Mul>, Store<Div> >, Value> {};
  struct Add: Char<'+'> {};
  struct Sub: Char<'-'> {};
  struct Sum: Op<Or<Store<Add>, Store<Sub> >, Store<Product> > {};
  struct Expr: Store<Sum> {};
  struct BracedExpr: Seq<Char<'('>, Expr, Char<')'> > {};
  struct MathExpr: Seq<Expr, Eof> {};

  int EvalNode(TextParser::Node* node);
  void PrintNode(TextParser::Node* node, int offset = 0);
}

#endif  // CALC_GRAMMAR_H
