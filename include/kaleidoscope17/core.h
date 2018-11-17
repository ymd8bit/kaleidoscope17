#ifndef KALEIDOSCOPE17_CORE_H
#define KALEIDOSCOPE17_CORE_H

#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace kaleidoscope17 {

struct Core
{
  llvm::LLVMContext ctx;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::Module> mod;
  std::map<std::string, llvm::Value *> named_values;

  Core() : ctx{}, builder{ctx}, mod{}, named_values{} {}
};

} // namespace kaleidoscope17

#endif
