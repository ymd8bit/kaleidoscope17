#ifndef KALEIDOSCOPE_DRIVER_H
#define KALEIDOSCOPE_DRIVER_H

#include "kaleidoscope17/parser.h"
#include "kaleidoscope17/token.h"

#include <cstdio>
#include <iostream>

namespace kaleidoscope17 {

class Driver final
{
public:
  Driver() = default;
  ~Driver() = default;

  Driver(std::istream &strm) : parser_{strm} {};

  void mainloop()
  {
    while (true) {
      std::cout << "Kaleidoscope >> ";
      auto token = parser_.get_next_token();

      switch (token.type) {
        case TokenType::Eof:
          std::cout << "EOF" << std::endl;
          return;
        case TokenType::Semicolon:
          break;
        case TokenType::Def:
          handle_def();
          break;
        case TokenType::Extern:
          handle_extern();
          break;
        default:
          /// primary :: = identifier_expr
          ///         :: = number_expr
          ///         :: = paren_expr
          /// parse expression ::= primary binop_rhs
          /// binop_rhs ::= ('+' primary)*
          handle_top_level_expr();
          break;
      }
    }
  }

private:
  Parser parser_;

  void handle_def()
  {
    if (parser_.parse_def()) {
      std::cout << "Parsed a function definition." << std::endl;
    }
  }

  void handle_extern()
  {
    if (parser_.parse_extern()) {
      std::cout << "Parsed an extern." << std::endl;
    }
  }

  void handle_top_level_expr()
  {
    if (parser_.parse_top_level_expr()) {
      std::cout << "Parsed a top-level expr." << std::endl;
    }
  }
};

} // namespace kaleidoscope17

#endif