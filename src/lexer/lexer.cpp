
#include "lexer/lexer.hpp"
#include "util/logging.hpp"

namespace dude {
namespace lexer {

  // ctor
  Lexer::Lexer(File& file)
    : _file(file), _it(file.content().cbegin())
  {
  }


  //
  Token Lexer::next_token()
  {
    char ch = next_character();

    if (have_reached_eof()) {
      return Token( Token::Kind::EOF );
    }

    // Branch upon "special" single characters".
    switch(ch) {
      case '\0':
        return Token( Token::Kind::nullbyte );
    }

    // Other branch conditions :
    if (is_blank_character(ch)) {
      put_back_character(ch);
      return try_lex_a_bunch_of_blank_space();
    }

    return Token();
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

