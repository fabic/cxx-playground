#ifndef _DUDE_LEXER_TOKEN_H
#define _DUDE_LEXER_TOKEN_H

/// Damn C header -__-
///   “ /usr/include/libio.h:62:14: note: expanded from macro 'EOF'
///     # define EOF (-1) ”
#ifdef EOF
#  undef EOF
#endif

namespace dude {
namespace lexer {

  /**
   */
  struct Cursor {
    /// There's always a first line, even if file is empty, true ?
    int _line   = 1;
    int _column = 0;
  };


  /**
   */
  struct Selection {
    Cursor _start;
    Cursor _end;
  };

  /**
   */
  struct Line {
    Selection _selection;
  };

  /**
   */
  struct Token {
    public:
      using size_t = std::string::size_type;

      /** Token kinds.
       */
      enum Kind : unsigned short {
        NIL, EOF, nullbyte, blank, number, string, identifier,
        symbol, null_pointer, keyword
      };

    public:
      Kind   _kind;
      Selection _selection; // fixme: remove.
      /// Start offset within the source text buffer.
      size_t _offset;
      /// Character count composing the lexem.
      size_t _count;

    public:
      explicit Token(Kind k = Kind::NIL) : _offset(0), _count(0) {}

      Token(Kind k, size_t offset, size_t count)
        : _kind(k), _offset(offset), _count(count) {}

      bool is_nil() const { return _kind == Kind::NIL; }
      bool is_eof() const { return _kind == Kind::EOF; }
  };

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_TOKEN_H

