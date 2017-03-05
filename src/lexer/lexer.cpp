
#include "lexer/lexer.hpp"
#include "util/logging.hpp"

namespace dude {
namespace lexer {

  // ctor
  Lexer::Lexer(File& file)
    : _file(file),
      _it(file.content().cbegin()),
      _it_end(file.content().cend())
  {
  }


  //
  Token Lexer::next_token()
  {
    if (have_reached_eof()) {
      return Token( Token::Kind::EOF );
    }

    char ch = next_character();

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
    else if (ch == '/') {
      char nch = next_character();
      if (nch == '*') {
        put_back_character(nch);
        put_back_character( ch);
        return try_lex_comment_block();
      } else {
        put_back_character(nch);
        return Token(Token::Kind::slash);
      }
    }
    // ERROR - Consume any character we do not know,
    //         and form a 'whatever' token with it.
    else {
      do {
        switch(ch) {
          case '\0':
          case ' ': case '\t': case '\n': case '\r':
            ;
          case '/':
          default:
            ch = next_character();
        }
      } while( true );
      put_back_character(ch);
      return Token(Token::Kind::whatever);
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
      if (! is_blank_character(ch)) {
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


  Token
    Lexer::try_lex_comment_block()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      char ch = next_character();
      char nch = next_character();

      if (ch != '/' && nch != '*') {
        put_back_character(nch);
        put_back_character( ch);
        throw dude::ex::yet_undefined_exception(
            "TODO: try_lex_comment_block(): impl. backtracking ?");
      }

      while( ! have_reached_eof() )
      {
        ch = next_character();

        if (ch == '*') {
          nch = next_character();
          if (nch == '/') {
            logtrace << "Lexer: got one comment block !";
            break;
          }
        }

      }

      // todo: err. handling in case we reached EOF before matching the
      //       actual comment block end "*/" ?
      lexem_end_ptr = &*_it;

      return Token(Token::Kind::comment, lexem_start_ptr, lexem_end_ptr);
    }


  std::string
    Token::raw_text() const
    {
      return std::string(cbegin(), cend());
    }

} // lexer ns.
} // dude ns.

