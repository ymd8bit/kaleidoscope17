#ifndef KALEIDOSCOPE17_DRIVER_H
#define KALEIDOSCOPE17_DRIVER_H

#include "kaleidoscope17/core.h"
#include "kaleidoscope17/parser.h"
#include "kaleidoscope17/token.h"
#include "kaleidoscope17/visitor.h"

#include <cstdio>
#include <iostream>

namespace kaleidoscope17 {

class Driver final
{
private:
  Parser parser_;
  Core core_;

public:
  Driver() = default;
  ~Driver() = default;

  Driver(std::istream &strm) : parser_{strm}, core_{} {};

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

  void handle_def()
  {
    if (auto func_ptr = parser_.parse_def()) {
      std::cout << "Parsed a function definition." << std::endl;
    }
  }

  void handle_extern()
  {
    if (auto proto_ptr = parser_.parse_extern()) {
      std::cout << "Parsed an extern." << std::endl;
    }
  }

  void handle_top_level_expr()
  {
    auto func_ptr = parser_.parse_top_level_expr();
    if (func_ptr) {
      std::cout << "Parsed a top-level expr." << std::endl;
    }

    // auto code_gen_visitor = CodeGenVisitor(&core_);
    // code_gen_visitor(*(func_ptr.get()));
  }
};

} // namespace kaleidoscope17

#endif