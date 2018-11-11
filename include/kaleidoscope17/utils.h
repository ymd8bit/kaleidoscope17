#ifndef KALEIDOSCOPE_UTILS_H
#define KALEIDOSCOPE_UTILS_H

#include "kaleidoscope17/ast.h"
#include "kaleidoscope17/logger.h"
#include "kaleidoscope17/token.h"

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

namespace kaleidoscope17
{

void print_token(Token &token);
const std::string expr_ptr_name(ExprPtr &exptr);
void print_expr_ptr(ExprPtr &exptr);

} // namespace kaleidoscope17

#endif