#include "dumbster/parser.hpp"
# include "util/logging.hpp"

namespace dude {
namespace dumbster {

  Parser::Parser(xfs::path fileName)
    : _file(fileName),
      _alex(_file),
      current_token_it_(_tokens.begin()),
      fragments_(nullptr)
  { }


  Token&
    Parser::next_token()
    {
      current_token_it_++;

      if (current_token_it_ == _tokens.end()) {
        Token tok = _alex.next_token();
        _tokens.push_back(std::move(tok));
        current_token_it_--;
      }

      return *current_token_it_ ;
    }


  Token&
    Parser::rewind_token()
    {
      --current_token_it_ ;

      if (current_token_it_ == _tokens.end()) {
        throw dude::ex::yet_undefined_exception(
            "Parser::rewind_token(): can't rewind past the beginning lad -_-");
      }

      return *current_token_it_ ;
    }


  Token&
    Parser::token()
    {
      if (current_token_it_ == _tokens.end()) {
        throw dude::ex::yet_undefined_exception(
            "Parser::token(): Huh! We've got _no_ current token !");
      }

      return *current_token_it_;
    }


  bool
    Parser::matchSymbol(char ch)
    {
      return token().is_symbol(ch);
    }


  void
    Parser::pushExpectedSymbol(char * mch)
    {
      //_stack.emplace_front(Token::Kind::symbol, mch);
      pushExpectedToken(Token::Kind::symbol, mch);
    }


  bool
    Parser::isExpected(const Token& tok)
    {
      for(const Token& kot : _stack) {
        if (tok == kot)
          return true;
      }

      return false;
    }


  void
    Parser::parse()
    {
      logtrace << "Parser::parse(): begin.";

      assert(fragments_ == nullptr);

      pushExpectedToken(Token::Kind::EOF);

      fragments_ = new Fragment();

      nt_whatever(fragments_, 1);

      while( true )
      {
        Token& tok = next_token();

        if (tok.is_eof()) {
          loginfo << "Parser reached EOF, bye...";
          break;
        }
        else if (tok.is_nil()) {
          logerror << "Parser got a NIL token, stopping now.";
          break;
        }
        else if (tok.is_null_byte()) {
          logwarn << "Parser got a \\0 byte, ignoring it.";
        }
        else if (tok.is_blank()) {
          logtrace << "Parser ate some blanks -_-";
        }
        else if (tok.is_symbol('{')) {
          //enter_parse_block();
        }
        else {
          logwtf << "Got an unexpected token, token kind : "
                 << static_cast<short>(tok._kind);
        }
      }
      logtrace << "Parser::parse(): end.";
    }


  void
    Parser::nt_whatever(Fragment *previous, int depth)
    {
      logtrace << "nt_whatever: start.";

      while(true)
      {
        Fragment *current = new Fragment();

        current->set_previous_fragment(previous);
        previous->set_next_fragment(current);

        while (true)
        {
          Token& tok = next_token();

          if (isExpected(tok)) {
            rewind_token();
            logdebug << "nt_whatever: matched an expected token.";
            goto my_first_goto_in_a_while;
          }

          current->push_token(&tok);

          if (tok.is_eof()) {
            loginfo << "nt_whatever(): reached EOF, bye...";
            goto my_first_goto_in_a_while;
          }
          else if (tok.is_blank()) {
            loginfo << "nt_whatever(): skipping blank(s) token."
                    << tok.text();
          }
          else if (tok.is_symbol(';')) {
            current->set_kind(Fragment::Kind::statement);
            previous = current;
            logtrace << "nt_whatever: got one fragment !";
            break;
          }
          // Enter block.
          else if (tok.is_symbol('{')) {
            current->set_kind(Fragment::Kind::block_antecedent);
            current->pop_token();
            rewind_token();
            Fragment * block = nt_curly_block(current, depth+1);
            current->set_next_fragment( block );
            block->set_previous_fragment( current );
          }
          else if (tok.is_symbol('}')) {
          }
        }
      }

my_first_goto_in_a_while:
      logtrace << "nt_whatever: finished.";
    }


  Fragment *
    Parser::nt_curly_block(Fragment *preceding, int depth)
    {
      logtrace << "nt_curly_block: begin.";

      Token& tok = next_token();

      assert( matchSymbol('{') );

      Fragment *block = new Fragment(Fragment::Kind::block);
      block->push_token(&tok);

      while(true) {
        tok = next_token();

        nt_whatever(block, depth+1);

        if (tok.is_eof()) {
          logdebug << "nt_curly_block: reached eof.";
          break;
        }
        else if (tok.is_symbol('}')) {
          logdebug << "nt_curly_block: matched '}' end-of-block.";
          break;
        }

        //nt_whatever();
      }

      logtrace << "nt_curly_block: end.";

      return block;
    }


  void
    Parser::debug_print_ast(std::ostream& os)
    {
      loginfo << "Parser::debug_print_ast(): START.";

      Fragment *current = fragments_;

      do {
        os << "FRAGMENT KIND: " << (int)current->kind();
        auto tokens = current->tokens();
        for(Token *tok : tokens) {
          //loginfo << "» Token: " << tok->text();
          os << tok->text();
        }
      } while( (current = current->next()) != nullptr );

      loginfo << "Parser::debug_print_ast(): END.";
    }


  void
    Parser::dev_parse()
    {
      logtrace << "Parser::parse(): begin.";

      Lexer alex (_file);

      while( true )
      {
        //Token tok = alex.next_token();
        Token& tok = next_token();

        if (tok.is_eof()) {
          loginfo << "Parser reached EOF.";
          break;
        }
        else if (tok.is_nil()) {
          logwarn << "Parser got a NIL token";
          break;
        }
        else if (tok.is_null_byte()) {
          logwarn << "Parser got a \\0 byte, ignoring it.";
        }
        else if (tok.is_blank()) {
          loginfo << "Parser got blanks.";
        }
        else if (tok.is_symbol()) {
          loginfo << "Parser got a symbol :"
                  << tok.first_character();
        }
        else if (tok.is_identifier()) {
          loginfo << "Parser got an identifier : "
                  << tok.text();
        }
        else if (tok.is_number()) {
          loginfo << "Parser got a number : "
                  << tok.text();
        }
        else if (tok.is_string()) {
          loginfo << "Parser got a string : "
                  << tok.text();
        }
        else if (tok.is_whatever()) {
          logwarn << "Parser got a “whatever” token from the lexer :"
                  << tok.raw_text();
        }
        else if (tok.is_comment()) {
          loginfo << "Parser got a comment (!) : \n"
                  //<< tok.text() ;
                  << tok.raw_text() ;
        }
        else {
          logwtf << "Got an unexpected token: "
                 << static_cast<short>(tok._kind);
        }
      }

      logtrace << "Parser::parse(): end.";
    }

} // psr ns.
} // dude ns.
