
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

  bool // static btw.
    Lexer::is_symbol_character(char ch)
    {
      switch(ch) {
        case '/': case '*': case '#': case '@': case '^': case '%': case '&':
        case '|': case '.': case ':': case '-': case '+': case '>': case '<':
        case '=': case '(': case ')': case '~': case '!': case '"': case '\'':
        case '`': case '[': case ']': case '{': case '}': case '$': case ';':
        case ',': case '\\':
          return true;
        default:
          return false;
      }
    }

  Token
    Lexer::next_token()
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

      // Consume blank space.
      if (is_blank_character(ch)) {
        put_back_character(ch);
        return try_lex_a_bunch_of_blank_space();
      }
      // Single slash '/' symbol, comment block `/* ... */`,
      // multiple consecutive lines of `// ...` comments.
      else if (ch == '/') {
        char nch = next_character();
        // ¿ Start of a comment block ?
        if (nch == '*') {
          put_back_character(nch);
          put_back_character( ch);
          return try_lex_comment_block();
        } else {
          put_back_character(nch);
          return Token(Token::Kind::symbol, &*_it, 1);
        }
      }
      // Double-quoted "..." string :
      else if (ch == '"') {
        put_back_character(ch);
        return try_lex_double_quoted_string();
      }
      // Symbols
      else if (is_symbol_character(ch)) {
        return Token(Token::Kind::symbol, &_it[-1], 1);
      }
      // Identifier
      else if (may_character_start_identifier(ch)) {
        put_back_character(ch);
        return try_lex_identifier();
      }
      // ERROR - Consume any character we do not know,
      //         and form a 'whatever' token with it.
      else {
        const char * ptr_start = &*_it;
        const char * ptr_end   = nullptr;
        do {
          switch(ch) {
            case '\0':
            case ' ': case '\t': case '\n': case '\r':
              ;
            case '/':
            default:
              ch = next_character();
          }
        } while( !have_reached_eof());
        ptr_end = &*_it;
        put_back_character(ch);
        return Token(Token::Kind::whatever, ptr_start, ptr_end);
      }

      return Token();
    }


  Token
    Lexer::try_lex_a_bunch_of_blank_space()
    {
      const char * blank_start_ptr = &*_it;
      const char * blank_end_ptr   = nullptr;

      do {
        char ch = next_character();
        if (! is_blank_character(ch)) {
          put_back_character(ch);
          break;
        }
      } while( true );

      blank_end_ptr = &*_it;

      return Token(Token::Kind::blank, blank_start_ptr, blank_end_ptr);
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


  Token
    Lexer::try_lex_identifier()
    {
      const char * lexem_start_ptr = &_it[0];
      const char * lexem_end_ptr   = nullptr;

      while( ! have_reached_eof() )
      {
        char ch = next_character();
        if (!is_digit_character(ch)
            && !may_character_start_identifier(ch)) {
          put_back_character(ch);
          break;
        }
      }

      lexem_end_ptr = &_it[0];

      return Token(Token::Kind::identifier, lexem_start_ptr, lexem_end_ptr);
    }

  Token
    Lexer::try_lex_double_quoted_string()
    {
      const char * lexem_start_ptr = &_it[0];
      const char * lexem_end_ptr   = nullptr;

      // Consume start of string character '"' :
      char ch = next_character();
      if (ch != '"') {
        throw dude::ex::yet_undefined_exception(
            "Trying to lex a double-quoted string, "
            "but first character is not '\"'" );
      }

      char nch = '\0';

      while( ! have_reached_eof() )
      {
        ch = next_character();
        // Skip escaped double-quote \" :
        if (ch == '\\' && (nch = next_character()) == '"' )
          continue;
        else if (ch == '"')
          break;
      }

      lexem_end_ptr = &_it[0];

      return Token(Token::Kind::string, lexem_start_ptr, lexem_end_ptr);
    }

  bool // static btw.
    Lexer::may_character_start_identifier(char ch)
    {
      // See Clang's `Lexer::LexTokenInternal(...)`
      //     [src/clong/_clang/Lex/Lexer.cpp]
      switch(ch) {
        // C99 6.4.2: Identifiers.
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
          return true;
        default:
          return false;
      }
    }


  std::string
    Token::raw_text() const
    {
      return std::string(cbegin(), cend());
    }

  std::string
    Token::text() const
    {
      if (start_ == nullptr)
        throw dude::ex::yet_undefined_exception("Argh! Token#start_ is a nullptr !");
      else if (_count == 0)
        throw dude::ex::yet_undefined_exception("Argh! Token#count_ is 0 (no characters, wtf?).");

      if (is_comment()) {
        size_t i = 0, j = 0, k = 0;
        std::string retv (_count, '^');
        const char *str = start_;

        // Skip "/*"
        if (str[i] == '/') i++;
        if (str[i] == '*') i++;

        while(true)
        {
          // Skip leading "* " or "*\n".
          if (str[i] == '*' && (str[i+1] == ' ' || str[i+1] == '\n')) {
            i += 2;
          }

          // `i` will hold the start of this comment line.
          // Consume characters until the end-of-line.
          j = i;
          bool eoc = false;
          while (str[j] != '\n'
              && !(eoc = str[j] == '*' && str[j+1] == '/')
              // Bounds checking is ^ slightly incorrect here,
              // but we're supposed to meet the "*/" marking the
              // end comment block before we seg. fault.
              && j < _count)
          {
            j++;
          }

          // Consume the line-feed.
          if (!eoc) { j++; }
          // and copy to the destination buffer.
          retv.replace(k, j-i, str+i, j-i);
          k += j-i;

          // Reached comment's end ⇒ exit !
          if (str[j] == '*' && str[j+1] == '/') { break; }

          // Consumme leading whitespaces (imperfect).
          i = j;
          while (str[i] == ' ' && i < _count) { i++; }
          if (str[i] == '*' && str[i+1] != '/') { i++; }

          // and start over.
        }

        retv.resize(k);

        return retv;
      }

      return raw_text();
    }


  char
    Token::first_character() const
    {
      if (start_ == nullptr || _count == 0)
        throw dude::ex::yet_undefined_exception("No pointer or character count is 0.");
      return start_[0];
    }

} // lexer ns.
} // dude ns.

