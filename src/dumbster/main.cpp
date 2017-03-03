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
namespace psr {

  using dude::ex::yet_undefined_exception;


  /**
   */
  struct Cursor {
    int _line   = 1; // there's always a first line, even if file is empty.
    int _column = 0;
  };


  /**
   */
  struct Selection {
    Cursor _start;
    Cursor _end;
  };


  /**
   */
  struct Line {
    Selection _selection;
  };


  /**
   */
  class TextBuffer {
    public:
      using self = TextBuffer;
      using self_r = self & ;
      using self_p = self * ; // or std::shared_ptr<...>, inheritable_shared_from_this<>?
    protected:
      std::unique_ptr<char *> _buffer = nullptr;
      std::vector<Line> _lines;
      // fileName / source ?
    public:
      explicit TextBuffer();
      self_r ReadFile(std::string fileName);
  };

  /// Ctor
  TextBuffer::TextBuffer()
    : _lines(1)
  {  }

  ///
  typename TextBuffer::self_r
  TextBuffer::ReadFile(std::string fileName)
  {
    namespace fs = std::experimental::filesystem;
    auto fileSize = fs::file_size(fileName);
    std::ifstream _file;
    return *this;
  }


  //  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~






  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~


  /**
   */
  class Parser {
  protected:
    Lexer _lexer;
  public:
    explicit Parser(std::string fileName);
    void parse();
  };

  ///
  Parser::Parser(std::string fileName)
    : _lexer(fileName) {}

  ///
  void Parser::parse()
  {
    logtrace << "Parser says: let's lex it !";

    while(true)
    {
      Token tok = _lexer.next_token();

      if (tok.is_eof()) {
        logtrace << "We reached EOF";
        break;
      }
      else if (tok.is_nil()) {
        logerror << "Met a NIL token, lexer no like -_-";
      }

      std::cerr << '.';
    }

    logtrace << "Parser says: “I'm done, lad.”";
  }

  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~


  /**
   */
  class Fragment {
    protected:
      std::shared_ptr<Fragment> previous_;
      std::shared_ptr<Fragment> next_;
      unsigned int _line, _colunm;
      // list of lexed tokens with no semantics ?
  };


  /**
   */
  class Block : public Fragment {
    protected:
      std::shared_ptr<Fragment> inner_;
  };


  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~



} // psr ns.


/**
 * ! executable: dumbster
 */
int main(int argc, const char *argv[])
{
  using namespace std;
  using psr::Parser;

  logdebug << "Hey, I'm " << argv[0];


  for(int n = 1; n < argc; n++)
  {
    auto fileName = argv[n];
    Parser parser (fileName);
    logdebug << "~ ~ ~" << " FILE `" << fileName << "` ~ ~ ~" << endl;
    parser.parse();
  }

  logdebug << "Good bye...";

  return 0;
}
