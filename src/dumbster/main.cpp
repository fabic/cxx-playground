
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

namespace psr {


  /**
   */
  struct Token {
      unsigned int is_eof:1;
      unsigned int is_blank_space:1;
      unsigned int is_sharp:1;
      unsigned int is_double_colon:1;
      //unsigned int _unused:XXX;
    int _start_line, _end_line;
    int _start_col, _end_col;
    Token() : is_eof(false), is_blank_space(false), is_sharp(false), is_double_colon(false) {}
  };


  /**
   */
  class Parser {
  protected:
  protected:
    std::ifstream file;
    int _line = 1, _column = 0;
  public:
    void parseFile(std::string fileName);
    void beginParseFile();
    Token readToken();
  };


  /**
   */
  struct Cursor {
    int _lineStart = -1, _lineEnd = -1;
    int _columnStart = -1, _columnEnd = -1;
  };


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


  void Parser::parseFile(std::string fileName)
  {
    std::cout << "Hey! " << fileName << std::endl;

    file.open(fileName);

    if (file.fail()) {
      std::cerr << "Could not open file: " << fileName << std::endl;
      return;
    }

    beginParseFile();

    file.close();

    std::cout << "Bye! " << fileName << std::endl;
  }


  void Parser::beginParseFile()
  {
    Token token;

    while(! (token = readToken()).is_eof) {
      std::cout << "Read token" << std::endl;
    };
  }




  inline bool is_blank_character(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
  }


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

} // psr ns.


/**
 */
int main(int argc, const char *argv[])
{
  using namespace std;
  using psr::Parser;

  cout << "Hey, I'm " << argv[0] << endl;

  Parser parser;

  for(int n = 1; n < argc; n++)
  {
    auto fileName = argv[n];
    cout << "~ ~ ~" << " FILE `" << fileName << "` ~ ~ ~" << endl;
    parser.parseFile( fileName );
  }

  cout << "Good bye..." << endl << endl;

  return 0;
}
