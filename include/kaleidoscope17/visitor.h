#ifndef KALEIDOSCOPE17_VISITOR_H
#define KALEIDOSCOPE17_VISITOR_H

#include <iostream>
#include <memory>
#include <variant>

#include "kaleidoscope17/ast.h"
#include "kaleidoscope17/core.h"
#include "llvm/IR/Value.h"

namespace kaleidoscope17 {

class CodeGenVisitor
{
private:
  std::shared_ptr<Core> core_;

public:
  CodeGenVisitor(Core* core) : core_{core} {}

  void operator()(NumExprAST& expr_ast)
  {
    std::cout << "NumExprAST(" << expr_ast.num() << ")" << std::endl;
  }

  void operator()(VarExprAST& expr_ast)
  {
    std::cout << "VarExprAST(" << expr_ast.name() << ")" << std::endl;
  }

  void operator()(BinaryExprAST& binary_expr)
  {
    std::cout << "BinaryExprAST" << std::endl;
    std::cout << "arg1: ";
    std::visit(*this, binary_expr.lhs());
    std::cout << "arg2: ";
    std::visit(*this, binary_expr.rhs());
  }

  void operator()(CallExprAST& expr_ast)
  {
    std::cout << "CallExprAST" << std::endl;
  }

  void operator()(PrototypeAST& expr_ast)
  {
    std::cout << "PrototypeAST" << std::endl;
  }

  void operator()(FunctionAST& expr_ast)
  {
    std::cout << "FunctionAST" << std::endl;
  }
};

} // namespace kaleidoscope17

#endif
