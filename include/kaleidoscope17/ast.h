#ifndef KALEIDOSCOPE17_AST_H
#define KALEIDOSCOPE17_AST_H

#include "kaleidoscope17/token.h"

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace kaleidoscope17 {

class NumExprAST;
class VarExprAST;
class BinaryExprAST;
class CallExprAST;
class CallExprAST;
class PrototypeAST;
class FunctionAST;

using ExprAST = std::variant<NumExprAST, VarExprAST, BinaryExprAST, CallExprAST,
                             PrototypeAST, FunctionAST>;

using NumExprPtr = std::unique_ptr<NumExprAST>;
using VarExprPtr = std::unique_ptr<VarExprAST>;
using BinaryExprPtr = std::unique_ptr<BinaryExprAST>;
using CallExprPtr = std::unique_ptr<CallExprAST>;
using PrototypePtr = std::unique_ptr<PrototypeAST>;
using FunctionPtr = std::unique_ptr<FunctionAST>;
using ExprPtr = std::unique_ptr<ExprAST>;

class NumExprAST
{
private:
  num_t num_;

public:
  NumExprAST() = default;
  NumExprAST(num_t num) : num_{num} {};

  inline num_t num() const { return num_; }
};

class VarExprAST
{
private:
  std::string name_;

public:
  VarExprAST() = default;
  VarExprAST(const std::string &name) : name_{name} {}

  inline const std::string &name() const { return name_; }
};

class BinaryExprAST
{
private:
  char op_;
  std::unique_ptr<ExprAST> lhs_, rhs_;

public:
  BinaryExprAST(char op, ExprPtr lhs, ExprPtr rhs)
    : op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)}
  {}

  inline const auto &lhs() const { return *(lhs_.get()); }
  inline const auto &rhs() const { return *(rhs_.get()); }
  inline const auto op() const { return op_; }
};

class CallExprAST
{
private:
  std::string callee_;
  std::vector<std::unique_ptr<ExprAST>> args_;

public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
    : callee_{callee}, args_{std::move(args)}
  {}

  inline const auto &callee() const { return callee_; }
  inline const auto &args() const { return args_; }
};

class PrototypeAST
{
private:
  std::string name_;
  std::vector<std::string> arg_names_;

public:
  PrototypeAST() = default;
  ~PrototypeAST() = default;

  PrototypeAST(const std::string &name,
               const std::vector<std::string> &arg_names)
    : name_{name}, arg_names_{arg_names}
  {}

  inline const auto &name() const { return name_; }
  inline const auto &arg_names() const { return arg_names_; }
};

class FunctionAST
{
private:
  PrototypePtr proto_;
  ExprPtr body_;

public:
  FunctionAST() = default;
  ~FunctionAST() = default;

  FunctionAST(PrototypePtr proto, ExprPtr body)
    : proto_{std::move(proto)}, body_{std::move(body)}
  {}

  inline const auto &proto() const { return *(proto_.get()); }
  inline const auto &body() const { return *(body_.get()); }
  // inline auto &proto_unsafe() const { return *(proto_.get()); }
  // inline auto &body_unsafe() const { return *(body_.get()); }
};

} // namespace kaleidoscope17

#endif