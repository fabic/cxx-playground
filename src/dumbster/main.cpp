
#include <iostream>
#include <fstream>
#include <string>

namespace psr {

  struct Token {
    char character;
    union {
      unsigned int is_eof:1;
      unsigned int is_sharp:1;
      unsigned int _word;
    } kind;
    Token(char ch);
  };

  class Parser {
  protected:
  protected:
    std::ifstream file;
  public:
    void parseFile(std::string fileName);
    void beginParseFile();
    Token readToken();
  };

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
    auto token = readToken();
  }

  Token::Token(char ch) : character(ch)
  {
    switch(ch) {
    case '#':
      kind.is_sharp = true;
      break;
    }
  }

  Token Parser::readToken()
  {
    Token tok(-1);

    char ch = file.get();

    if (file.eof()) {
      tok.kind.is_eof = true;
      return tok;
    }



    return tok;
  }

} // psr ns.


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
