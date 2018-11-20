#include "kaleidoscope17/utils.h"
#include "kaleidoscope17/ast.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_os_ostream.h"

#include <string>
#include <variant>

namespace kaleidoscope17 {

void print_token(const Token &token) { std::cout << token.name() << std::endl; }

template <typename T>
inline const bool isa(ExprAST &expr)
{
  return std::holds_alternative<T>(expr);
}

const std::string expr_ptr_name(const ExprPtr &expr_ptr)
{
  ExprAST &expr = *(expr_ptr.get());
  if (isa<NumExprAST>(expr)) {
    return "NumExprAsT";
  } else if (isa<VarExprAST>(expr)) {
    return "VarExprAST";
  } else if (isa<BinaryExprAST>(expr)) {
    return "BinaryExprAST";
  } else if (isa<CallExprAST>(expr)) {
    return "CallExprAST";
  } else if (isa<PrototypeAST>(expr)) {
    return "PrototypeAST";
  } else if (isa<FunctionAST>(expr)) {
    return "FunctionAST";
  } else {
    assert(false && "Given ExprPtr type is not supported... ");
  }
}

void print_expr_ptr(const ExprPtr &expr_ptr)
{
  std::cout << expr_ptr_name(expr_ptr) << std::endl;
}

void print_llvm_value(llvm::Value *value, std::ostream &os)
{
  auto raw_os = llvm::raw_os_ostream(os);
  value->print(raw_os);
}

} // namespace kaleidoscope17
