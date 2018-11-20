#ifndef KALEIDOSCOPE17_DRIVER_H
#define KALEIDOSCOPE17_DRIVER_H

#include "kaleidoscope17/core.h"
#include "kaleidoscope17/parser.h"
#include "kaleidoscope17/token.h"
#include "kaleidoscope17/utils.h"
#include "kaleidoscope17/visitor.h"
#include "llvm/IR/Value.h"

#include <cstdio>
#include <iostream>

namespace kaleidoscope17 {

class Driver final
{
private:
  std::istream istrm_;
  std::ostream ostrm_;
  Parser parser_;
  Core core_;
  CodegenVisitor codegen_visitor_;
  PrintVisitor print_visitor_;
  const bool debug_mode_ = false;

public:
  Driver() = default;
  ~Driver() = default;

  Driver(std::istream& istrm, std::ostream& ostrm,
         const bool debug_mode = false)
    : istrm_{istrm.rdbuf()}
    , ostrm_{ostrm.rdbuf()}
    , parser_{istrm}
    , core_{}
    , codegen_visitor_{&core_}
    , print_visitor_{ostrm_}
    , debug_mode_{debug_mode} {};

  void mainloop()
  {
    while (true) {
      if (debug_mode_) {
        ostrm_ << "Kaleidoscope >> ";
      }
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

      if (debug_mode_) {
        ostrm_ << std::endl;
      }
    }
  }

  void handle_eof()
  {
    if (debug_mode_) {
      ostrm_ << "EOF" << std::endl;
    }
  }

  void handle_def()
  {
    auto func_ptr = parser_.parse_def();
    if (!func_ptr) {
      EXCEPTION("failed to parse define statement");
    }

    print_debug_info(func_ptr, "parsed a function definition.");
  }

  void handle_extern()
  {
    auto proto_ptr = parser_.parse_extern();
    if (!proto_ptr) {
      EXCEPTION("failed to parse extern statement");
    }

    print_debug_info(proto_ptr, "parsed an extern.");
  }

  void handle_top_level_expr()
  {
    auto func_ptr = parser_.parse_top_level_expr();
    if (!func_ptr) {
      EXCEPTION("failed to parse extern statement");
    }

    print_debug_info(func_ptr, "parsed a top-level expr.");
  }

  template <typename T>
  void print_debug_info(T& expr_ptr, const std::string& header)
  {
    if (debug_mode_) {
      ostrm_ << header << std::endl;
      ostrm_ << "\n";
      ostrm_ << "-- parsed result" << std::endl;
      print_parsed_result(expr_ptr);
      ostrm_ << "\n\n";
      ostrm_ << "-- generated LLVM IR" << std::endl;
      print_generated_llvm_ir(expr_ptr);
    }
  }

  template <typename T>
  void print_parsed_result(T& expr_ptr)
  {
    print_visitor_(*(expr_ptr.get()));
  }

  template <typename T>
  void print_generated_llvm_ir(T& expr_ptr)
  {
    auto value = codegen_visitor_(*(expr_ptr.get()));
    print_llvm_value(value, ostrm_);
  }
};

} // namespace kaleidoscope17

#endif