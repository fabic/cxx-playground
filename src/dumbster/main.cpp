/**
 * ~~! executable: dumbster~~ !! see main() for target def.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

# include "util/logging.hpp"
# include "lexer/lexer.hpp"
# include "filesystem/file.hpp"

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
    public:
      explicit Parser(xfs::path fileName);
      void parse();
  };

  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

  Parser::Parser(xfs::path fileName) : _file(fileName)
  {
  }

  void
    Parser::parse()
    {
      logtrace << "Parser::parse(): begin.";

      Lexer alex (_file);

      while( true )
      {
        Token tok = alex.next_token();

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
    parser.parse();
  }

  logdebug << "Good bye...";

  return 0;
}
