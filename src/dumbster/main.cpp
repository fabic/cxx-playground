
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

# include "util/exceptions.hpp"
# include "util/logging.hpp"

namespace psr {

  using dude::ex::yet_undefined_exception;


  /**
   */
  struct Cursor {
    int _line;
    int _column;
  };


  /**
   */
  struct Selection {
    Cursor _start;
    Cursor _end;
  };


  /**
   */
  struct Token {
    public:
      enum Kind : int {
        blank, number, string, identifier,
        symbol
      };
    public:
      Selection _selection;
      Kind      _kind;
  };


  /**
   */
  class Lexer {
    protected:
      std::string   _fileName;
      std::ifstream _file;
      int _line   = 1;
      int _column = 0;
    public:
      explicit Lexer(std::string fileName);
      Token next_token();
      static inline bool is_blank_character(char ch);
  };

  ///
  Lexer::Lexer(std::string fileName)
    : _fileName(fileName),
      _file(_fileName)
  {
    logtrace << "Opening file " << _fileName;

    _file.open(fileName);

    if (_file.fail())
      throw yet_undefined_exception();
  }

  ///
  bool is_blank_character(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
  }


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
    while(true)
    {
      _lexer.next_token();
    }
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


#if 0
  Token Parser::readToken() // todo: ren. nextToken().
  {
    Token token;

    char ch = file.get();

    bool reached_eof = file.eof();

    if (reached_eof) {
      token.is_eof = true;
      return token;
    }

    _column++;

    // Consume a block of blank space.
    if (is_blank_character(ch))
    {
      token._start_line = _line;
      token._start_col  = _column;

      reached_eof = false;

      while(is_blank_character((ch = file.get())))
      {
        reached_eof = file.eof();

        if (reached_eof) {
          break;
        }
        else if (ch == '\n') {
          _line++;
          _column = 0;
          continue;
        }

        _column++;
      }

      if (! reached_eof)
        file.putback(ch);

      token._end_line = _line;
      token._end_col  = _column;

      return token;
    }

    // Consume whatever stuff we do not know about.
    token._start_line = _line;
    token._start_col  = _column;

    while(! is_blank_character((ch = file.get())))
    {
      reached_eof = file.eof();

      if (reached_eof) {
        break;
      }

      _column++;
    }

    if (! reached_eof)
      file.putback(ch);

    token._end_line = _line;
    token._end_col  = _column;

    return token;
  }
#endif // 0

} // psr ns.


/**
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
