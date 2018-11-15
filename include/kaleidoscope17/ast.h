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
using ProtoPtr = std::unique_ptr<PrototypeAST>;
using FuncPtr = std::unique_ptr<FunctionAST>;
using ExprPtr = std::unique_ptr<ExprAST>;

class NumExprAST
{
public:
  NumExprAST() = default;
  NumExprAST(num_t num) : num_{num} {};

private:
  num_t num_;
};

class VarExprAST
{
public:
  VarExprAST() = default;
  VarExprAST(const std::string &name) : name_{name} {}

private:
  std::string name_;
};

class BinaryExprAST
{
public:
  BinaryExprAST(char op, ExprPtr lhs, ExprPtr rhs)
    : op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)}
  {}

private:
  char op_;
  std::unique_ptr<ExprAST> lhs_, rhs_;
};

class CallExprAST
{
public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
    : callee_{callee}, args_{std::move(args)}
  {}

private:
  std::string callee_;
  std::vector<std::unique_ptr<ExprAST>> args_;
};

class PrototypeAST
{
public:
  PrototypeAST() = default;
  ~PrototypeAST() = default;

  PrototypeAST(const std::string &name, std::vector<std::string> args)
    : name_{name}, args_{args}
  {}

private:
  std::string name_;
  std::vector<std::string> args_;
};

class FunctionAST
{
public:
  FunctionAST() = default;
  ~FunctionAST() = default;

  FunctionAST(ProtoPtr proto, ExprPtr body)
    : proto_{std::move(proto)}, body_{std::move(body)}
  {}

private:
  ProtoPtr proto_;
  ExprPtr body_;
};

// using NodeAST = std::variant<NumExprAST, BinaryExprAST>;

// using BinaryExprPtr = std::unique_ptr<BinaryExprAST>;

} // namespace kaleidoscope17

#endif