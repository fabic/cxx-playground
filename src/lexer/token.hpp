#ifndef _DUDE_LEXER_TOKEN_H
#define _DUDE_LEXER_TOKEN_H

/// Damn C header -__-
///   “ /usr/include/libio.h:62:14: note: expanded from macro 'EOF'
///     # define EOF (-1) ”
#ifdef EOF
#  undef EOF
#endif

namespace dude {

  /**
   */
  struct Token {
    public:
      enum Kind : int {
        NIL, EOF, blank, number, string, identifier,
        symbol, null_pointer, keyword
      };
    public:
      Selection _selection;
      Kind      _kind;
    public:
      Token() : _kind(Kind::NIL) {}
      bool is_nil() const { return _kind == Kind::NIL; }
      bool is_eof() const { return _kind == Kind::EOF; }
  };

} // dude ns.

#endif // _DUDE_LEXER_TOKEN_H

