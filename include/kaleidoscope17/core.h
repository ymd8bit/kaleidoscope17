#ifndef KALEIDOSCOPE17_CORE_H
#define KALEIDOSCOPE17_CORE_H

#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace kaleidoscope17 {

class Core
{
private:
  llvm::LLVMContext ctx_;
  llvm::IRBuilder<> builder_;
  std::unique_ptr<llvm::Module> mod_;
  std::map<std::string, llvm::Value *> named_values_;

public:
  Core() : ctx_{}, builder_{ctx_}, mod_{}, named_values_{} {}
};

} // namespace kaleidoscope17

#endif
