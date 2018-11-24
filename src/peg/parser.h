#ifndef PEG_PARSER_H
#define PEG_PARSER_H

#include <cstring>

#include "peg/ast.h"

namespace peg {
  template<typename TokenT, typename IterT = const TokenT*>
  class Parser;
  class TextParser;
}

template<typename TokenT, typename IterT>
class peg::Parser {
public:
  typedef IterT Iterator;
  typedef Ast<Iterator> Tree;
  typedef class Tree::AbstractNode Node;

  Parser() = default;
  Parser(Iterator begin, Iterator end): iter_(begin), end_(end) {}
  Iterator iter() { return iter_; }
  void set_iter(Iterator iter) { iter_ = iter; }
  bool eof() { return iter_ >= end_; }
  void GotoNext() { ++iter_; }

  template<class RuleT>
  bool Parse(Iterator begin, Iterator end) {
    iter_ = begin;
    end_ = end;
    ast_.Clear();
    bool out = RuleT::Match(*this);
    if (!out) ast_.Clear();
    return out;
  }

  Node* ast_root() { return ast_.root(); }
  template<class RuleT>
  void CreateNode() { ast_.template CreateNode<RuleT>(iter_); }
  void CompleteNode() { ast_.CompleteNode(iter_); }
  void AbandonNode() { ast_.AbandonNode(); }
protected:
  Iterator iter_ = nullptr;
  Iterator end_ = nullptr;
  Tree ast_;
};

class peg::TextParser: public Parser<char> {
public:
  using Parser::Parser;
  template<class RuleT>
  bool Parse(const char* s) {
    return Parser::Parse<RuleT>(s, s + std::strlen(s));
  }
};

#endif  // PEG_PARSER_H
