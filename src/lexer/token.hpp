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
        NIL = 0, EOF, nullbyte, whatever, blank, number,
        string, identifier, symbol, null_pointer, keyword,
        slash, backslash, star, comment
      };

    public:
      Kind   _kind;
      Selection _selection; // fixme: remove.
      /// Start offset within the source text buffer.
      size_t _offset;

      const char * start_ = nullptr;

      /// Character count composing the lexem.
      size_t _count;
    public:
      explicit Token(Kind k = Kind::NIL)
        : _kind(k), _offset(0), _count(0) {}

      Token(Kind k, size_t offset, size_t count)
        : _kind(k), _offset(offset), _count(count) {}

      Token(Kind k, const char *start, const char *end)
        : _kind(k), start_(start), _count(end-start) {}

      bool is_nil() const { return _kind == Kind::NIL; }
      bool is_eof() const { return _kind == Kind::EOF; }
      bool is_null_byte() const { return _kind == Kind::nullbyte; }
      bool is_whatever() const { return _kind == Kind::whatever; }
      bool is_comment() const { return _kind == Kind::comment; }

      const char * cbegin() const { return start_; }
      const char * cend() const { return start_ + _count; }

      std::string raw_text() const;
      std::string text() const;
  };

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_TOKEN_H

