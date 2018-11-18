#ifndef KALEIDOSCOPE17_CORE_H
#define KALEIDOSCOPE17_CORE_H

#include <map>
#include <memory>
#include <string>

#include "kaleidoscope17/utils.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

namespace kaleidoscope17 {

struct Core
{
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::Value*> named_values;

  Core()
    : context{}
    , builder{context}
    , module{std::make_unique<llvm::Module>("kaleidoscope17", context)}
    , named_values{}
  {}

  llvm::Value* get_value(const std::string& name)
  {
    if (named_values.count(name)) {
      return named_values[name];
    } else {
      EXCEPTION(name + " is not registerd in named_values...");
    }
  }

  void set_value(const std::string& name, llvm::Value* value)
  {
    if (!named_values.count(name)) {
      named_values[name] = value;
    } else {
      EXCEPTION(name + " is already registerd in named_values...");
    }
  }
};

} // namespace kaleidoscope17

#endif
