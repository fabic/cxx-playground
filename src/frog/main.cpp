/**
 * ~~! executable: dumbster~~ !! see main() for target def.
 */

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <memory>

# include "util/exceptions.hpp"
# include "util/logging.hpp"

// psr abbr. "parser".
namespace frog {

  using dude::ex::yet_undefined_exception;

  class Frog {
    public:
      using self = Frog& ;
    protected:
    public:
      explicit Frog();
      self parseSourceFile(std::string filename);
  };

  // ctor
  Frog::Frog() {}

  //
  Frog::self
    Frog::parseSourceFile(std::string filename)
    {
      return *this;
    }

} // frog ns.


/**
 * ! executable: dumbster
 */
int main(int argc, const char *argv[])
{
  using namespace std;
  using namespace frog;

  logdebug << "Hey, I'm " << argv[0];

  Frog frog;

  for(int n = 1; n < argc; n++)
  {
    auto fileName = argv[n];
    logdebug << "~ ~ ~" << " FILE `" << fileName << "` ~ ~ ~" << endl;
    frog.parseSourceFile( fileName );
  }

  logdebug << "Good bye...";

  return 0;
}
