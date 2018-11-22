#ifndef PEG_AST_H
#define PEG_AST_H

// The Ast contains Ast::AbstractNode objects
// which contain a type_info struct to identify
// the rule type (i.e. the RuleT) that they are associated with.
// The template parameter "IterT" is the iterator associated with the input.
// Nodes  are created automatically by inserting Store<T> productions
// into a grammar.
namespace peg {
  template<typename IterT>
  struct Ast;
}

template<typename IterT>
struct peg::Ast {
  struct AbstractNode {
  };
};

#endif  // PEG_AST_H
