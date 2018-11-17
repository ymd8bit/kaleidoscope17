#ifndef KALEIDOSCOPE17_VISITOR_H
#define KALEIDOSCOPE17_VISITOR_H

#include <iostream>
#include <memory>
#include <variant>

#include "kaleidoscope17/ast.h"
#include "kaleidoscope17/core.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"

namespace kaleidoscope17 {

class PrintVisitor
{
public:
  PrintVisitor() = default;

  void operator()(const NumExprAST& ast)
  {
    std::cout << "NumExprAST(" << ast.num() << ")";
  }

  void operator()(const VarExprAST& ast)
  {
    std::cout << "VarExprAST(" << ast.name() << ")";
  }

  void operator()(const BinaryExprAST& ast)
  {
    std::cout << "BinaryExprAST(";
    std::visit(*this, ast.lhs());
    std::cout << " " << ast.op() << " ";
    std::visit(*this, ast.rhs());
    std::cout << ")";
  }

  void operator()(const CallExprAST& ast)
  {
    std::cout << "CallExprAST{";
    std::cout << ast.callee();
    std::cout << "(";
    const auto& args = ast.args();
    for (size_t i = 0; i < args.size(); i++) {
      std::visit(*this, *(args[i].get()));
      if (i != args.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ")}";
  }

  void operator()(const PrototypeAST& ast)
  {
    std::cout << "PrototypeAST{";
    std::cout << ast.name() << "(";
    const auto& arg_names = ast.arg_names();
    for (size_t i = 0; i < arg_names.size(); i++) {
      std::cout << arg_names[i];
      if (i != arg_names.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ")}";
  }

  void operator()(const FunctionAST& ast)
  {
    std::cout << "FunctionAST{";
    (*this)(ast.proto());
    std::visit(*this, ast.body());
    std::cout << "}";
  }
};

class CodegenVisitor
{
private:
  Core* core_;

public:
  CodegenVisitor(Core* core) : core_{core} {};

  // llvm::Value* operator()(const NumExprAST& ast)
  // {
  //   return llvm::ConstantFP::get(core_->ctx, llvm::APFloat(ast.num()));
  // }

  void operator()(const NumExprAST& ast) {}

  void operator()(const VarExprAST& ast)
  {
    std::cout << "VarExprAST(" << ast.name() << ")";
  }

  void operator()(const BinaryExprAST& ast)
  {
    std::cout << "BinaryExprAST(";
    std::visit(*this, ast.lhs());
    std::cout << " " << ast.op() << " ";
    std::visit(*this, ast.rhs());
    std::cout << ")";
  }

  void operator()(const CallExprAST& ast)
  {
    std::cout << "CallExprAST{";
    std::cout << ast.callee();
    std::cout << "(";
    const auto& args = ast.args();
    for (size_t i = 0; i < args.size(); i++) {
      std::visit(*this, *(args[i].get()));
      if (i != args.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ")}";
  }

  void operator()(const PrototypeAST& ast)
  {
    std::cout << "PrototypeAST{";
    std::cout << ast.name() << "(";
    const auto& arg_names = ast.arg_names();
    for (size_t i = 0; i < arg_names.size(); i++) {
      std::cout << arg_names[i];
      if (i != arg_names.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ")}";
  }

  void operator()(const FunctionAST& ast)
  {
    std::cout << "FunctionAST{";
    (*this)(ast.proto());
    std::visit(*this, ast.body());
    std::cout << "}";
  }
};

} // namespace kaleidoscope17

#endif
