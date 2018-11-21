#ifndef PEG_PARSER_H
#define PEG_PARSER_H

namespace peg {
  template<typename TokenT, typename IterT = const TokenT*>
  struct BasicParser;
  struct TextParser;
}

template<typename TokenT, typename IterT>
struct peg::BasicParser {
  typedef IterT Iterator;

  BasicParser() = default;
  BasicParser(Iterator begin, Iterator end): iter_(begin), end_(end) {}
  Iterator iter() { return iter_; }
  void set_iter(Iterator iter) { iter_ = iter; }
  bool eof() { return iter_ >= end_; }
  void GotoNext() { ++iter_; }
protected:
  Iterator iter_ = nullptr;
  Iterator end_ = nullptr;
};

struct peg::TextParser: peg::BasicParser<char> {
  using BasicParser::BasicParser;
};

#endif  // PEG_PARSER_H
