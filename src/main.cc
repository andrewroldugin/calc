#include <iostream>

#include "calc/grammar.h"

using std::endl;

void PrintHelp() {
  std::cout << "Usage: calc [<OPTION>]" << endl
            << "Calculate math expression." << endl
            << "Only number 0-9 and symbols */+-() are allowed." << endl
            << endl
            << "  -a, --ast print AST" << endl
            << "  -h, --help print help" << endl;
}

int main(int argc, char* argv[]) {
  bool ast = false;
  if (argc > 1) {
    auto option = argv[1];
    if (!std::strcmp(option, "-h") || !std::strcmp(option, "--help")) {
      PrintHelp();
      return 0;
    }
    if (!std::strcmp(option, "-a") || !std::strcmp(option, "--ast")) {
      ast = true;
    } else {
      std::cerr << "Unknown option: " << option << endl;
      PrintHelp();
    }
  }
  std::cout << "Enter math expression followed by Enter key" << endl
            << "or Ctrl-C to exit" << endl;
  peg::TextParser p;
  std::string s;
  while (std::getline(std::cin, s)) {
    if (p.Parse<calc::MathExpr>(s.c_str())) {
      std::cout << s << " = " << calc::EvalNode(p.ast_root()->child()) << endl;
      if (ast) calc::PrintNode(p.ast_root()->child());
    } else {
      std::cout << "The input `" << s << "` is invalid" << endl;
    }
  }
  return 0;
}
