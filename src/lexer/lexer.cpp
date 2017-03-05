
#include "lexer/lexer.hpp"
#include "util/logging.hpp"

namespace dude {
namespace lexer {

  // ctor
  Lexer::Lexer(std::istream& file)
    : _file(file),
      _source_text()
  {
    _source_text.reserve(initial_buffer_size);
    _lines.reserve(initial_line_count_storage);
  }


  //
  bool Lexer::is_blank_character(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
  }


  //
  char Lexer::next_character()
  {
    char ch = _file.get();

    // is this ok ?
    if (_file.eof()) {
      return 0;
    }

    return ch;
  }


  //
  void Lexer::put_back_character(char ch)
  {
    _file.putback( ch );
  }


  //
  Token Lexer::next_token()
  {
    char ch = next_character();

    if (_file.eof()) {
      return Token( Token::Kind::EOF );
    }

    // Branch upon "special" single characters".
    switch(ch) {
      case 0x00:
        return Token( Token::Kind::nullbyte );
    }

    // Other branch conditions :
    if (is_blank_character(ch)) {
      put_back_character(ch);
      return try_lex_a_bunch_of_blank_space();
    }
  }


  //
  Token Lexer::try_lex_a_bunch_of_blank_space()
  {
    Token toki;

    toki._selection._start = _cursor;

    logtrace << "Lexing blanks from ["
             << toki._selection._start._line   << ','
             << toki._selection._start._column << ']';

    do {
      char ch = next_character();
      if (ch == 0)
        break; // EOF
      else if (! is_blank_character(ch)) {
        put_back_character(ch);
        break;
      }
    } while( true );

    toki._selection._end = _cursor;

    // todo: check if we consumed anything at all.

    toki._kind = Token::Kind::blank;

    logtrace << "Lexed blanks end ["
             << toki._selection._end._line   << ','
             << toki._selection._end._column << ']';

    return toki;
  }

} // lexer ns.
} // dude ns.

