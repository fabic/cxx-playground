#ifndef _DUDE_LEXER_LEXER_H
#define _DUDE_LEXER_LEXER_H

#include <string>
#include <istream>
#include <vector>
#include "lexer/token.hpp"

namespace dude {
namespace lexer {

  /**
   */
  class Lexer {
    public:
      /// Have an initial buffer for about 2 times 50 lines.
      static constexpr int initial_line_count_storage = 2*100;
      /// Assuming lines of “80” characters = 8000 bytes.
      static constexpr int initial_buffer_size = 80 * initial_line_count_storage;
    protected:
      std::istream&      _file;
      std::string        _source_text;
      std::vector<Line>  _lines;
      Cursor             _cursor;
    public:
      /**
       * * FIXME: we do strange things there setting the `istream` buffer
       *   to the `_source_text` buffer – see impl. notes :
       *   <http://en.cppreference.com/w/cpp/io/basic_filebuf/setbuf>
       */
      explicit Lexer(std::istream& file);
      inline char next_character();
      inline void put_back_character(char ch); // rewind() ?
      Token next_token();
      static inline bool is_blank_character(char ch);
      Token try_lex_a_bunch_of_blank_space();
  };

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_LEXER_H
