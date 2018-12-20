#ifndef KALEIDOSCOPE17_CORE_H
#define KALEIDOSCOPE17_CORE_H

#include <map>
#include <memory>
#include <string>

#include "kaleidoscope17/KaleidoscopeJIT.h" //  release 70
#include "kaleidoscope17/utils.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

namespace kaleidoscope17 {

struct Core
{
  using Module = llvm::Module;
  using FunctionPassManager = llvm::legacy::FunctionPassManager;
  using Jit = llvm::orc::KaleidoscopeJIT;

  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<Module> module;
  std::unique_ptr<FunctionPassManager> fpm;
  std::map<std::string, llvm::Value*> named_values;
  std::unique_ptr<Jit> jit;
  bool pass_enable;

  Core(const bool _pass_enable = false)
    : context{}
    , builder{context}
    , module{std::make_unique<Module>("kaleidoscope17", context)}
    , fpm{std::make_unique<FunctionPassManager>(module.get())}
    , named_values{}
    , pass_enable{_pass_enable}
  {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    jit = std::make_unique<Jit>();

    if (pass_enable) {
      init_fpm();
    }
  }

  llvm::Value* get_value(const std::string& name)
  {
    if (named_values.count(name)) {
      return named_values[name];
    } else {
      EXCEPTION(name + " is not registered in named_values...");
    }
  }

  void set_value(const std::string& name, llvm::Value* value)
  {
    if (!named_values.count(name)) {
      named_values[name] = value;
    } else {
      EXCEPTION(name + " is already registered in named_values...");
    }
  }

  void apply_passes(llvm::Function* func) { fpm->run(*func); }

  void init()
  {
    init_module();
    init_fpm();
  }

private:
  void init_module()
  {
    module->setDataLayout(jit->getTargetMachine().createDataLayout());
  }

  void init_fpm()
  {
    // Do simple "peephole" optimizations and bit-twiddling optimizations.
    fpm->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    fpm->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    fpm->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    fpm->add(llvm::createCFGSimplificationPass());
    fpm->doInitialization();
  }
};

} // namespace kaleidoscope17

#endif
