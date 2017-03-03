#ifndef _DUDE_LEXER_H
#define _DUDE_LEXER_H

#include <string>
#include <fstream>
#include "lexer/token.hpp"

namespace dude {

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

} // dude ns.

#endif // _DUDE_LEXER_H
