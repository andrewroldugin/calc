#ifndef PEG_GRAMMAR_H
#define PEG_GRAMMAR_H

namespace peg {
  // Or attempts to match each of the passed rules until
  // one is found successful.
  template<class FirstRuleT, class SecondRuleT, class... RestRulesT>
  struct Or {
    template<class ParserT>
    static bool Match(ParserT& p) {
      return FirstRuleT::template Match(p) || SecondRuleT::template Match(p)
        || (... || RestRulesT::template Match(p));
    }
  };
}

#endif  // PEG_GRAMMAR_H
