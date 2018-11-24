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
  typedef IterT Iterator;

  template<class RuleT>
  class TypedNode;

  class AbstractNode {
  public:
    AbstractNode() = default;
    // the node is not completely constructed until the children-nodes
    // are manually added and Complete(...) is called
    AbstractNode(Iterator begin, AbstractNode* parent):
      begin_(begin), parent_(parent) {
    }

    virtual ~AbstractNode() {
      Clear();
    }

    void AddChild(AbstractNode* child) {
      *last_child_ptr_ = child;
      last_child_ptr_ = &child->next_;
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

    // This function only allows deletion of the last child in a list.
    void DeleteChild(AbstractNode* node) {
      if (node == child_) {
        child_ = nullptr;
        last_child_ptr_ = &child_;
      } else {
        auto before_last = child_;
        while (before_last->next() != node) before_last = before_last->next();
        before_last->next_ = nullptr;
        last_child_ptr_ = &before_last->next_;
      }
      delete node;
    }

    // return true if the associated rule type matches the template parameter
    template<typename T>
    bool TypeMatches() {
      return GetRuleTypeInfo() == typeid(T);
    }

    void Clear() {
      delete child_; child_ = nullptr;
      delete next_; next_ = nullptr;
      last_child_ptr_ = &child_;
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

  // CreateNode is called when an attempt is made to match a Store rule
  template<class RuleT>
  void CreateNode(Iterator begin) {
    current_ = current_->template NewChild<RuleT>(begin);
  }

  // Complete is called when a Store rule is successfully matched
  void CompleteNode(Iterator end) {
    current_->Complete(end);
    current_ = current_->parent();
  }

  // AbandonNode is called when a Store rule fails to match
  void AbandonNode() {
    auto node = current_;
    current_ = current_->parent();
    current_->DeleteChild(node);
  }

  void Clear() {
    root_.Clear();
  }

  AbstractNode* root() { return &root_; }
private:
  TypedNode<void> root_;
  AbstractNode* current_ = &root_;
};

#endif  // PEG_AST_H
