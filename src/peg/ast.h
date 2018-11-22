#ifndef PEG_AST_H
#define PEG_AST_H

#include <typeinfo>

// The Ast contains Ast::AbstractNode objects
// which contain a type_info struct to identify
// the rule type (i.e. the RuleT) that they are associated with.
// The template parameter "IterT" is the iterator associated with the input.
// Nodes  are created automatically by inserting Store<T> productions
// into a grammar.
namespace peg {
  template<typename IterT>
  class Ast;
}

template<typename IterT>
class peg::Ast {
public:
  template<class RuleT>
  class TypedNode;

  class AbstractNode {
  public:
    typedef IterT Iterator;

    AbstractNode() = default;
    // the node is not completely constructed until the children-nodes
    // are manually added and Complete(...) is called
    AbstractNode(Iterator begin, AbstractNode* parent):
      begin_(begin), parent_(parent) {
    }

    virtual ~AbstractNode() {
      delete child_;
      delete next_;
    }

    void AddChild(AbstractNode* child) {
      *last_child_ptr_ = child;
      auto next = child->next();
      last_child_ptr_ = &next;
    }

    template<class RuleT>
    TypedNode<RuleT>* NewChild(Iterator begin) {
      auto node = new TypedNode<RuleT>(begin, this);
      AddChild(node);
      return node;
    }

    void Complete(Iterator end) {
      end_ = end;
    }

    void DeleteChild(AbstractNode* child) {
      if (child == child_) {
        delete child;
        child_ = nullptr;
        last_child_ptr_ = &child_;
      }
    }

    virtual const std::type_info& GetRuleTypeInfo() = 0;

    AbstractNode* child() { return child_; }
    AbstractNode* next() { return next_; }
    AbstractNode* parent() { return parent_; }
    Iterator begin() { return begin_; }
    Iterator end() { return end_; }
  private:
    Iterator begin_ = nullptr;
    Iterator end_ = nullptr;
    AbstractNode* next_ = nullptr;
    AbstractNode* child_ = nullptr;
    AbstractNode* parent_ = nullptr;
    AbstractNode** last_child_ptr_ = &child_;
  };  // AbstractNode

  template<class RuleT>
  class TypedNode: public AbstractNode {
  public:
    using AbstractNode::AbstractNode;
    const std::type_info& GetRuleTypeInfo() override {
      return typeid(RuleT);
    }
  };

  // CreateNode is called when an attempt is made to match a
  // Store production rule
  AbstractNode* root() { return &root_; }
  template<class RuleT, class ParserT>
  void CreateNode(ParserT& p) {
    current_ = current_->template NewChild<RuleT>(p.iter());
  }

  // Complete is called when a Store production rule
  // is successfully matched
  template<class ParserT>
  void CompleteNode(ParserT& p) {
    current_->Complete(p.iter());
    current_ = current_->parent();
  }

  // AbandonNode is called when a Store production rule fails to match
  template<class ParserT>
  void AbondonNode(ParserT&){
    auto node = current_;
    current_ = current_->parent();
    current_->DeleteChild(node);
  }
private:
  TypedNode<void> root_;
  AbstractNode* current_ = &root_;
};

#endif  // PEG_AST_H
