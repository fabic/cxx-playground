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
    ;

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
