#ifndef PEG_GRAMMAR_H
#define PEG_GRAMMAR_H

namespace peg {
  // Or attempts to match each of the passed rules until
  // one is found successful.
  template<class FirstRuleT, class SecondRuleT, class... RestRulesT>
  struct Or;

  // Seq attempts to match each of the passed rules, sequentially until
  // they all pass. If one fails, the input is reset to the original position
  template<class... RulesT>
  struct Seq;
}

// Or attempts to match each of the passed rules until
// one is found successful.
template<class FirstRuleT, class SecondRuleT, class... RestRulesT>
struct peg::Or {
  template<class ParserT>
  static bool Match(ParserT& p) {
    return FirstRuleT::template Match(p) || SecondRuleT::template Match(p)
      || (... || RestRulesT::template Match(p));
  }
};

// Seq attempts to match each of the passed rules, sequentially until
// they all pass. If one fails, the input is reset to the original position
template<class... RulesT>
struct peg::Seq {
  template<class ParserT>
  static bool Match(ParserT& p) {
    auto iter = p.iter();
    bool out = (... && RulesT::template Match(p));
    if (!out) p.set_iter(iter);
    return out;
  }
};

#endif  // PEG_GRAMMAR_H
