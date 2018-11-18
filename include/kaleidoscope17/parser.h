#ifndef KALEIDOSCOPE17_PARSER_H
#define KALEIDOSCOPE17_PARSER_H

#include "kaleidoscope17/ast.h"
#include "kaleidoscope17/lexer.h"
#include "kaleidoscope17/logger.h"
#include "kaleidoscope17/token.h"
#include "kaleidoscope17/utils.h"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <variant>

namespace kaleidoscope17 {

class Parser
{
  Token cur_token_;
  Lexer lexer_;
  std::map<char, int> binop_precedence_;

public:
  Parser() = delete;
  ~Parser() = default;

  Parser(std::istream &istrm) : lexer_{istrm} { set_binop_precedence_(); }

  Token get_current_token() { return cur_token_; }

  Token get_next_token()
  {
    cur_token_ = lexer_.get_token();
    // print_token(cur_token_);
    return cur_token_;
  }

  template <class T, class... Args>
  ExprPtr make_expr(Args &&... args)
  {
    return std::make_unique<ExprAST>(T{std::forward<Args>(args)...});
  }

  /// number_expr ::= number
  ExprPtr parse_num_expr()
  {
    ExprPtr num_expr = make_expr<NumExprAST>(get_current_num());
    get_next_token(); // this just consume the next number
    return num_expr;
  }

  /// paren_expr ::= '(' expression ')'
  ExprPtr parse_paren_expr()
  {
    get_next_token(); // eat left parenthesis '('
    auto v = parse_expr();
    if (!v) {
      return nullptr;
    }
    if (current_token_is_not(TokenType::ParenR)) {
      EXCEPTION("expected ')' here.");
    }
    return v;
  }

  /// identifier_expr ::= identifier
  ///                 ::= identifier '(' expression ')'
  ExprPtr parse_id_expr()
  {
    std::string id_name = get_current_id_str();
    ExprPtr id_expr = make_expr<VarExprAST>(id_name);

    get_next_token(); // eat the next identifier

    // this means a variable found, not a function.
    if (current_token_is_not(TokenType::ParenL)) {
      return make_expr<VarExprAST>(id_name);
    }

    get_next_token(); // eat '('
    std::vector<ExprPtr> args;

    // if current expression were function call
    // this would have one or more arguments.
    if (current_token_is_not(TokenType::ParenR)) {
      while (true) {
        // push the parsed argument into arguments list.
        auto arg = parse_expr();
        args.push_back(std::move(arg));

        // found the end of the argument.
        if (current_token_is(TokenType::ParenR)) {
          break;
        }

        if (current_token_is_not(TokenType::Comma)) {
          EXCEPTION("Expected ')' or ',' in argument list...");
        }
        get_next_token(); // eat ','
      }
    }

    get_next_token(); // eat ')'.
    return make_expr<CallExprAST>(id_name, std::move(args));
  }

  /// primary :: = identifier_expr
  ///         :: = number_expr
  ///         :: = paren_expr
  ExprPtr parse_primary()
  {
    switch (get_current_token().type) {
      default:
        EXCEPTION("Unknown primary token found...");
      case TokenType::Id:
        return parse_id_expr();
      case TokenType::Num:
        return parse_num_expr();
      case TokenType::ParenL:
        return parse_paren_expr();
    }
  }

  /// parse expression ::= primary binop_rhs
  ExprPtr parse_expr()
  {
    auto lhs = parse_primary();
    return parse_binop_rhs(0, std::move(lhs));
  }

  /// binop_rhs ::= ('+' primary)*
  ExprPtr parse_binop_rhs(int expr_prec, ExprPtr lhs)
  {
    // if this is a binop, look for its precedence.
    while (true) {
      int token_prec = get_current_token_precedence();

      // if this is a binop that binds at least as tightly as the current binop,
      // consume it, otherwise we are done.
      if (token_prec < expr_prec) {
        return lhs;
      }
      // now we know the current is a binop, then extracting the char like '+'.
      char op = get_current_char();
      get_next_token(); // eat binop

      // get the next primary as rhs
      auto rhs = parse_primary();

      int next_prec = get_current_token_precedence();
      if (token_prec < next_prec) {
        rhs = parse_binop_rhs(token_prec + 1, std::move(rhs));
      }
      // merge lhs / rhs
      lhs = make_expr<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
    }
  }

  /// prototype ::= id '(' id* ')'
  PrototypePtr parse_proto()
  {
    if (current_token_is_not(TokenType::Id)) {
      EXCEPTION("Expected function name in prototype...");
    }

    std::string func_name = get_current_id_str();
    get_next_token(); // eat identifier TokenType::ParenL '('.

    if (current_token_is_not(TokenType::ParenL)) {
      EXCEPTION("Expected '(' in prototype...");
    }

    // read the list of argument names.
    std::vector<std::string> arg_names;

    // read next token, which should be ')' or the 1st argument ID
    get_next_token();

    // if current expression has one or more arguments.
    if (current_token_is_not(TokenType::ParenR)) {
      while (true) {
        if (current_token_is(TokenType::Id)) {
          // push parsed arg into args list.
          arg_names.push_back(get_current_id_str());
        } else {
          EXCEPTION("Expected ID after '(' in prototype...");
        }

        get_next_token();

        // found the end of the argument.
        if (current_token_is(TokenType::ParenR)) {
          break;
        }

        if (current_token_is_not(TokenType::Comma)) {
          EXCEPTION("Expected ')' or ',' in prototype...");
        } else {
          get_next_token(); // eat ','
        }
      }
    }

    // we need to eat ')'.
    get_next_token();

    return std::make_unique<PrototypeAST>(func_name, arg_names);
  }

  /// definition ::= 'def' prototype expression
  FunctionPtr parse_def()
  {
    get_next_token();
    auto proto = parse_proto();

    if (auto expr = parse_expr()) {
      return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    } else {
      EXCEPTION("Expected expression in definition.");
    }
  }

  /// extern ::= 'extern' prototype
  PrototypePtr parse_extern()
  {
    get_next_token(); // eat 'extern'.
    return parse_proto();
  }

  /// toplevel_expr ::= expression
  FunctionPtr parse_top_level_expr()
  {
    if (auto expr = parse_expr()) {
      auto proto =
        std::make_unique<PrototypeAST>("", std::vector<std::string>());
      return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    } else {
      EXCEPTION("Expected expression at top level.");
      return nullptr;
    }
  }

private:
  // inline ExprPtr log_error(const std::string& msg)
  // {
  //   Logger::Fatal(msg);
  //   return nullptr;
  // }

  // inline PrototypePtr log_error_proto(const std::string& msg)
  // {
  //   Logger::Fatal(msg);
  //   return nullptr;
  // }

  ExprPtr parse_error(const std::string &msg)
  {
    Logger::Debug(msg);
    return nullptr;
  }

  inline const Token get_current_token() const { return cur_token_; }

  inline char get_current_char()
  {
    assert(current_token_is(TokenType::GenericAscii));
    return get_token_value<char>(cur_token_);
  }

  inline std::string get_current_id_str()
  {
    assert(current_token_is(TokenType::Id));
    return get_token_value<std::string>(cur_token_);
  }

  inline num_t get_current_num()
  {
    assert(current_token_is(TokenType::Num));
    return get_token_value<num_t>(cur_token_);
  }

  inline bool current_token_is(TokenType type) const
  {
    return get_current_token().type == type;
  }

  inline bool current_token_is_not(TokenType type) const
  {
    return !current_token_is(type);
  }

  /// get the precedence for each binary operator token.
  int get_current_token_precedence()
  {
    if (current_token_is_not(TokenType::GenericAscii)) {
      return -1; // -1 means less than any other precedences.
    }

    char cur_binop = get_current_char();
    int token_prec = binop_precedence_[cur_binop];

    if (token_prec <= 0) {
      return -1; // same above.
    }
    return token_prec;
  }

  void set_binop_precedence_()
  {
    binop_precedence_['<'] = 10;
    binop_precedence_['+'] = 20;
    binop_precedence_['-'] = 20;
    binop_precedence_['*'] = 40;
  }
};

} // namespace kaleidoscope17

#endif