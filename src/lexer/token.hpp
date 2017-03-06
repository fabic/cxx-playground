#ifndef _DUDE_LEXER_TOKEN_H
#define _DUDE_LEXER_TOKEN_H

#include <string>
#include <util/exceptions.hpp>

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
        slash_symbol, backslash_symbol, star_symbol, comment
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

      Token(Kind k, const char *start, size_t count)
        : _kind(k), start_(start), _count(count) {}

      bool is_nil() const noexcept { return _kind == Kind::NIL; }
      bool is_eof() const noexcept { return _kind == Kind::EOF; }
      bool is_blank() const noexcept { return _kind == Kind::blank; }
      bool is_null_byte() const noexcept { return _kind == Kind::nullbyte; }
      bool is_whatever() const noexcept { return _kind == Kind::whatever; }
      bool is_comment() const noexcept { return _kind == Kind::comment; }
      bool is_symbol() const noexcept { return _kind == Kind::symbol; }
      bool is_identifier() const noexcept { return _kind == Kind::identifier; }
      bool is_string() const noexcept { return _kind == Kind::string; }

      /**
       * Test if this token is a single character symbol matching `ch`.
       */
      bool is_symbol(char ch) const {
        if (!is_symbol()) return false;
        else if (start_ == nullptr) {
          throw dude::ex::yet_undefined_exception(
              "Symbol token it is, but no pointer"
              " into the originating buffer !" );
        }
        else if (_count == 0) {
          throw dude::ex::yet_undefined_exception(
              "Symbol token with a valid pointer into some buffer"
              ", but character count is zero" );
        }
        else return start_[0] == ch;
      }

      const char * cbegin() const { return start_; }
      const char * cend() const { return start_ + _count; }

      std::string raw_text() const;
      std::string text() const;
      char first_character() const;
  };

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_TOKEN_H

