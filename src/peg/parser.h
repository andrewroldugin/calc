#ifndef PEG_PARSER_H
#define PEG_PARSER_H

#include "peg/ast.h"

namespace peg {
  template<typename TokenT, typename IterT = const TokenT*>
  struct Parser;
  typedef Parser<char> TextParser;
}

template<typename TokenT, typename IterT>
struct peg::Parser {
  typedef IterT Iterator;
  typedef Ast<Iterator> Tree;
  typedef class Tree::AbstractNode Node;

  Parser(Iterator begin, Iterator end): iter_(begin), end_(end) {}
  Iterator iter() { return iter_; }
  void set_iter(Iterator iter) { iter_ = iter; }
  bool eof() { return iter_ >= end_; }
  void GotoNext() { ++iter_; }

  Node* ast_root() { return ast_.root(); }
  template<class RuleT>
  void CreateNode() { ast_.template CreateNode<RuleT>(iter_); }
  void CompleteNode() { ast_.CompleteNode(iter_); }
  void AbondonNode() { ast_.AbondonNode(); }
protected:
  Iterator iter_ = nullptr;
  Iterator end_ = nullptr;
  Tree ast_;
};

#endif  // PEG_PARSER_H
