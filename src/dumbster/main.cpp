/**
 * ~~! executable: dumbster~~ !! see main() for target def.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <list>

# include "util/logging.hpp"
# include "lexer/lexer.hpp"
# include "filesystem/file.hpp"
# include "dumbster/fragments.hpp"

// psr abbr. "parser".
namespace dude {
namespace dumbster {

  using dude::ex::yet_undefined_exception;
  using dude::fs::File;

  using dude::lexer::Lexer;
  using dude::lexer::Token;

  namespace xfs = boost::filesystem;

  /**
   */
  class Parser {
    protected:
      File _file;
      Lexer _alex;
      std::list<Token> _tokens;
      Fragment *fragments_;
    public:
      explicit Parser(xfs::path fileName);
      void parse();
      void dev_parse();
      Token& next_token();
      void nt_whatever(Fragment *previous);
      void nt_block();
      void debug_print_ast();
  };

  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

  Parser::Parser(xfs::path fileName)
    : _file(fileName),
      _alex(_file),
      fragments_(nullptr)
  { }


  Token&
    Parser::next_token()
    {
      Token tok = _alex.next_token();
      _tokens.push_back(std::move(tok));
      return _tokens.back();
    }


  void
    Parser::parse()
    {
      logtrace << "Parser::parse(): begin.";

      assert(fragments_ == nullptr);

      fragments_ = new Fragment();

      nt_whatever(fragments_);

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
    Parser::nt_whatever(Fragment *previous)
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

          current->push_token(&tok);

          if (tok.is_eof()) {
            loginfo << "nt_whatever(): reached EOF, bye...";
            goto my_first_goto_in_a_while;
          }
          else if (tok.is_symbol(';')) {
            current->set_kind(Fragment::Kind::statement);
            previous = current;
            logtrace << "nt_whatever: got one fragment !";
            break;
          }
        }
      }

my_first_goto_in_a_while:
      logtrace << "nt_whatever: finished.";
    }


  void
    Parser::debug_print_ast()
    {
      loginfo << "Parser::debug_print_ast(): START.";

      Fragment *current = fragments_;

      do {
        loginfo << "Fragment kind: " << (int)current->kind();
        auto tokens = current->tokens();
        for(Token *tok : tokens) {
          loginfo << "» Token: " << tok->text();
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


/**
 * ! executable: dumbster
 */
int main(int argc, const char *argv[])
{
  logdebug << "Hey, I'm " << argv[0];

  using dude::dumbster::Parser;

  for(int n = 1; n < argc; n++)
  {
    auto fileName = argv[n];
    logdebug << "~ ~ ~" << " FILE `" << fileName << "` ~ ~ ~";
    Parser parser (fileName);
    //parser.dev_parse();
    parser.parse();
    parser.debug_print_ast();
  }

  logdebug << "Good bye...";

  return 0;
}
