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
  const bool debug_mode_ = false;

public:
  Driver() = default;
  ~Driver() = default;

  Driver(std::istream &strm, const bool debug_mode = false)
    : parser_{strm}, core_{}, debug_mode_{debug_mode} {};

  void mainloop()
  {
    while (true) {
      std::cout << "Kaleidoscope >> ";
      auto token = parser_.get_next_token();

      switch (token.type) {
        case TokenType::Eof:
          handle_eof();
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

      std::cout << std::endl;
    }
  }

  void handle_eof()
  {
    if (debug_mode_) {
      std::cout << "EOF" << std::endl;
    }
  }

  void handle_def()
  {
    auto func_ptr = parser_.parse_def();
    if (!func_ptr) {
      EXCEPTION("failed to parse define statement");
    }

    if (debug_mode_) {
      std::cout << "parsed a function definition." << std::endl;
      auto code_gen_visitor = PrintVisitor();
      code_gen_visitor(*(func_ptr.get()));
    }
  }

  void handle_extern()
  {
    auto proto_ptr = parser_.parse_extern();
    if (!proto_ptr) {
      EXCEPTION("failed to parse extern statement");
    }

    if (debug_mode_) {
      std::cout << "parsed an extern." << std::endl;
      auto code_gen_visitor = PrintVisitor();
      code_gen_visitor(*(proto_ptr.get()));
    }
  }

  void handle_top_level_expr()
  {
    auto func_ptr = parser_.parse_top_level_expr();
    if (!func_ptr) {
      EXCEPTION("failed to parse extern statement");
    }

    if (debug_mode_) {
      std::cout << "parsed a top-level expr." << std::endl;
      auto code_gen_visitor = PrintVisitor();
      code_gen_visitor(*(func_ptr.get()));
    }
  }
};

} // namespace kaleidoscope17

#endif