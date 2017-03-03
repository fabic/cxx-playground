
#include "lexer/lexer.hpp"

namespace dude {

  ///
  Lexer::Lexer(std::string fileName)
    : _fileName(fileName)
  {
    logtrace << "Opening file " << _fileName;

    _file.open(fileName);

    if (_file.fail())
      throw yet_undefined_exception("Could not open that file.");
  }

  ///
  bool Lexer::is_blank_character(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
  }

  ///
  char Lexer::next_character()
  {
    char ch = _file.get();

    // is this ok ?
    if (_file.eof()) {
      return 0;
    }

    // We keep track of line returns as we go.
    // (keeping information about the previous line
    //  since we have to support rewinding characters).
    if (ch == '\n') {
      // _lines.emplace_back(_cursor._line);
      // _lines.push_back( _cursor );
      _cursor._line++;
      _cursor._column = 0;
    }

    _cursor._column++; // fixme: always ?

    return ch;
  }

  ///
  void Lexer::put_back_character(char ch)
  {
    _file.putback( ch );

    if (ch == '\n') {
      _cursor._line++;
      _cursor._column = 0;
    }

    _cursor._column--;
  }

  ///
  Token Lexer::next_token()
  {
    Token tok;

    char ch = next_character();

    if (_file.eof()) {
      tok._kind = Token::Kind::EOF;
      return tok;
    }

    if (is_blank_character(ch)) {
      put_back_character(ch);
      try_lex_a_bunch_of_blank_space();
    }

    return tok;
  }

  ///
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

} // dude ns.

