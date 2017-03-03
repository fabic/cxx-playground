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


  /// Damn C header -__-
  /// /usr/include/libio.h:62:14: note: expanded from macro 'EOF'
  /// # define EOF (-1)
# ifdef EOF
#   undef EOF
# endif

  /**
   */
  struct Token {
    public:
      enum Kind : int {
        NIL, EOF, blank, number, string, identifier,
        symbol, null_pointer, keyword
      };
    public:
      Selection _selection;
      Kind      _kind;
    public:
      Token() : _kind(Kind::NIL) {}
      bool is_nil() const { return _kind == Kind::NIL; }
      bool is_eof() const { return _kind == Kind::EOF; }
  };


  /**
   */
  class Lexer {
    protected:
      std::string   _fileName;
      std::ifstream _file;
      //int _line   = 1; int _column = 0;
      Cursor _cursor;
      std::vector<Line> _lines;
    public:
      explicit Lexer(std::string fileName);
      inline char next_character();
      inline void put_back_character(char ch); // rewind() ?
      Token next_token();
      static inline bool is_blank_character(char ch);
      Token try_lex_a_bunch_of_blank_space();
  };

  ///
  Lexer::Lexer(std::string fileName)
    : _fileName(fileName)
  {
    logtrace << "Opening file " << _fileName;

    _file.open(fileName);

    if (_file.fail())
      throw yet_undefined_exception("Could not open that file.");
  }

  ///
  bool Lexer::is_blank_character(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
  }

  ///
  char Lexer::next_character()
  {
    char ch = _file.get();

    // is this ok ?
    if (_file.eof()) {
      return 0;
    }

    // We keep track of line returns as we go.
    // (keeping information about the previous line
    //  since we have to support rewinding characters).
    if (ch == '\n') {
      // _lines.emplace_back(_cursor._line);
      // _lines.push_back( _cursor );
      _cursor._line++;
      _cursor._column = 0;
    }

    _cursor._column++; // fixme: always ?

    return ch;
  }

  ///
  void Lexer::put_back_character(char ch)
  {
    _file.putback( ch );

    if (ch == '\n') {
      _cursor._line++;
      _cursor._column = 0;
    }

    _cursor._column--;
  }

  ///
  Token Lexer::next_token()
  {
    Token tok;

    char ch = next_character();

    if (_file.eof()) {
      tok._kind = Token::Kind::EOF;
      return tok;
    }

    if (is_blank_character(ch)) {
      put_back_character(ch);
      try_lex_a_bunch_of_blank_space();
    }

    return tok;
  }

  ///
  Token Lexer::try_lex_a_bunch_of_blank_space()
  {
    Token toki;

    toki._selection._start = _cursor;

    logtrace << "Lexing blanks from ["
             << toki._selection._start._line   << ','
             << toki._selection._start._column << ']';

    do {
      char ch = next_character();
      if (ch == 0)
        break; // EOF
      else if (! is_blank_character(ch)) {
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
