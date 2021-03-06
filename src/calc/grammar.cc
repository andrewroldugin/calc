#include "calc/grammar.h"

#include <iostream>
#include <string>

void calc::PrintNode(TextParser::Node* node, int offset) {
  while (node) {
    std::cout << std::string(offset, '.')
              << std::string(node->begin(), node->end())
              << "."
              << std::endl
              << std::string(offset, '.') << node->GetRuleTypeInfo().name()
              << std::endl;
    PrintNode(node->child(),
              offset + node->end() - node->begin() + 2);
    node = node->next();
  }
}

int calc::EvalNode(TextParser::Node* node) {
  if (!node) return 0;
  int out = 0;
  if (node->TypeMatches<Number>())
    out = std::stoi(std::string(node->begin(), node->end()));
  else {
    auto operand = node->child();
    out = EvalNode(operand);
    while (auto op = operand->next()) {
      operand = op->next();
      if (op->TypeMatches<Add>()) {
        out += EvalNode(operand);
      } else if (op->TypeMatches<Sub>()) {
        out -= EvalNode(operand);
      } else if (op->TypeMatches<Mul>()) {
        out *= EvalNode(operand);
      } else {
        out /= EvalNode(operand);
      }
    }
  }
  return out;
}
