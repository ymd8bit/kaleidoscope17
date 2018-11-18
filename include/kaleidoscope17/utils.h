#ifndef KALEIDOSCOPE17_UTILS_H
#define KALEIDOSCOPE17_UTILS_H

#include "kaleidoscope17/ast.h"
#include "kaleidoscope17/logger.h"
#include "kaleidoscope17/token.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_os_ostream.h"

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

namespace kaleidoscope17 {

void print_token(Token &token);
const std::string expr_ptr_name(ExprPtr &exptr);
void print_expr_ptr(ExprPtr &exptr);
void print_llvm_value(llvm::Value *value, std::ostream &os);

} // namespace kaleidoscope17

#endif