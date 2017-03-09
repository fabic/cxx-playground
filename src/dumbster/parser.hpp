#ifndef _DUDE_DUMBSTER_PARSER_H
#define _DUDE_DUMBSTER_PARSER_H

# include "dumbster/fragments.hpp"
# include "lexer/lexer.hpp"
# include "filesystem/file.hpp"

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
      Lexer _alex;
      std::list<Token> _tokens;
      std::list<Token>::iterator current_token_it_;
      Fragment *fragments_;
    public:
      explicit Parser(xfs::path fileName);
      void parse();
      void dev_parse();

      Token& next_token();
      Token& rewind_token();

      void nt_whatever(Fragment *previous, int depth);
      Fragment * nt_curly_block(Fragment *preceding, int depth);
      void debug_print_ast(std::ostream& os);
  };

} // dumbster ns.
} // dude ns.

#endif // _DUDE_DUMBSTER_PARSER_H
