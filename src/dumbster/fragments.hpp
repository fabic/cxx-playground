#ifndef _DUDE_DUMBSTER_FRAGMENTS_H
#define _DUDE_DUMBSTER_FRAGMENTS_H

#include <memory>
#include <list>
# include "lexer/token.hpp"
# include "util/logging.hpp"

namespace dude {
namespace dumbster {

  using dude::lexer::Token;

  /**
   */
  class Fragment {
    public:
      enum Kind : short {
        NIL = 0, statement, block, parenthesized, bracketized
      };
      using tokens_list_ref_t = std::list<Token *> & ;
    protected:
      Fragment *previous_ = nullptr;
      Fragment *next_     = nullptr;
      Fragment *inner_    = nullptr;
      std::list<Token *> _tokens;
      Kind _kind;
    public:
      explicit Fragment() : _kind(Kind::NIL) {}
      inline void push_token(Token *tok);
      inline void set_kind(Kind k) { _kind = k; }
      inline void set_previous_fragment(Fragment *frag);
      inline void set_next_fragment(Fragment *frag);
      inline Kind kind() const { return _kind; }
      inline Fragment * next() { return next_; }
      tokens_list_ref_t tokens() { return _tokens; }
  };


  /**
   */
  class Block : public Fragment {
    protected:
      //std::shared_ptr<Fragment> inner_;
  };

  void Fragment::push_token(Token *tok)
  {
    _tokens.push_back(tok);
  }

  void
    Fragment::set_previous_fragment(Fragment *prev)
    {
      if (previous_ != nullptr)
        logwarn << "Fragment's previous ptr is not null o_o";
      previous_ = prev;
    }

  void
    Fragment::set_next_fragment(Fragment *nex)
    {
      if (next_ != nullptr)
        logwarn << "Fragment's next ptr is not null o_o";
      next_ = nex;
    }

} // dumbster ns.
} // dude ns.

#endif // _DUDE_DUMBSTER_FRAGMENTS_H
