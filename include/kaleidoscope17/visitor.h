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
#include "llvm/IR/Verifier.h"

namespace kaleidoscope17 {

class PrintVisitor
{
private:
  std::ostream ostrm_;

public:
  PrintVisitor(std::ostream& ostrm) : ostrm_{ostrm.rdbuf()} {};

  void operator()(const NumExprAST& ast)
  {
    ostrm_ << "NumExprAST(" << ast.num() << ")";
  }

  void operator()(const VarExprAST& ast)
  {
    ostrm_ << "VarExprAST(" << ast.name() << ")";
  }

  void operator()(const BinaryExprAST& ast)
  {
    ostrm_ << "BinaryExprAST(";
    std::visit(*this, ast.lhs());
    ostrm_ << " " << ast.op() << " ";
    std::visit(*this, ast.rhs());
    ostrm_ << ")";
  }

  void operator()(const CallExprAST& ast)
  {
    ostrm_ << "CallExprAST{";
    ostrm_ << ast.callee();
    ostrm_ << "(";
    const auto& args = ast.args();
    for (size_t i = 0; i < args.size(); i++) {
      std::visit(*this, *(args[i].get()));
      if (i != args.size() - 1) {
        ostrm_ << ", ";
      }
    }
    ostrm_ << ")}";
  }

  void operator()(const PrototypeAST& ast)
  {
    ostrm_ << "PrototypeAST{";
    ostrm_ << ast.name() << "(";
    const auto& arg_names = ast.arg_names();
    for (size_t i = 0; i < arg_names.size(); i++) {
      ostrm_ << arg_names[i];
      if (i != arg_names.size() - 1) {
        ostrm_ << ", ";
      }
    }
    ostrm_ << ")}";
  }

  void operator()(const FunctionAST& ast)
  {
    ostrm_ << "FunctionAST{";
    (*this)(ast.proto());
    std::visit(*this, ast.body());
    ostrm_ << "}";
  }
};

class CodegenVisitor
{
private:
  Core* core_;

public:
  CodegenVisitor(Core* core) : core_{core} {};

  llvm::Value* operator()(const NumExprAST& ast)
  {
    return llvm::ConstantFP::get(core_->context, llvm::APFloat(ast.num()));
  }

  llvm::Value* operator()(const VarExprAST& ast)
  {
    return core_->get_value(ast.name());
  }

  llvm::Value* operator()(const BinaryExprAST& ast)
  {
    auto* lhs_op = std::visit(*this, ast.lhs());
    auto* rhs_op = std::visit(*this, ast.rhs());

    if (!lhs_op) {
      EXCEPTION("LHS operand is invalid...");
    }

    if (!rhs_op) {
      EXCEPTION("RHS operand is invalid...");
    }

    switch (ast.op()) {
      case '+':
        return core_->builder.CreateFAdd(lhs_op, rhs_op, "add_f");
      case '-':
        return core_->builder.CreateFSub(lhs_op, rhs_op, "sub_f");
      case '*':
        return core_->builder.CreateFMul(lhs_op, rhs_op, "mul_f");
      case '<':
        return core_->builder.CreateFCmpULT(lhs_op, rhs_op, "cmp_lt_f");
      default:
        EXCEPTION("Invalid binary operator is called...");
    }
  }

  llvm::Value* operator()(const CallExprAST& ast)
  {
    auto* mod = core_->module.get();
    auto callee_f = mod->getFunction(ast.callee());
    if (!callee_f) {
      EXCEPTION("Unknown function is referenced...");
    }

    if (callee_f->arg_size() != ast.args().size()) {
      EXCEPTION("Number of args don't match...");
    }

    std::vector<llvm::Value*> arg_values;
    for (auto& arg : ast.args()) {
      auto arg_value = std::visit(*this, *(arg.get()));
      arg_values.push_back(arg_value);
    }

    return core_->builder.CreateCall(callee_f, arg_values, "call_f");
  }

  llvm::Value* operator()(const PrototypeAST& ast)
  {
    // Now, we support only double type as a number
    std::vector<llvm::Type*> arg_types(ast.arg_names().size(),
                                       llvm::Type::getDoubleTy(core_->context));

    auto* func_type = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(core_->context), arg_types, false);

    auto* mod = core_->module.get();
    auto* func = llvm::Function::Create(
      func_type, llvm::Function::ExternalLinkage, ast.name(), mod);

    // set arguments' name to llvm::Function object.
    auto arg_name_ref = ast.arg_names().begin();
    for (auto& arg : func->args()) { arg.setName(*arg_name_ref++); }

    return func;
  }

  llvm::Value* operator()(const FunctionAST& ast)
  {
    // First, check for an existing function from a previous 'extern'
    // declaration.
    auto* mod = core_->module.get();
    auto& name = ast.proto().name();
    auto* func = mod->getFunction(name);

    if (!func) {
      func = static_cast<decltype(func)>((*this)(ast.proto()));
    }

    if (!func) {
      EXCEPTION("This prototype isn't defined yet...");
    }

    if (!func->empty()) {
      EXCEPTION("Function cannot be redefined...");
    }

    // create a new basic block to start insertion into.
    auto* bb = llvm::BasicBlock::Create(core_->context, "entry", func);
    core_->builder.SetInsertPoint(bb);

    for (auto& arg : func->args()) { core_->set_value(arg.getName(), &arg); }

    if (llvm::Value* ret = std::visit(*this, ast.body())) {
      // set a return value to the function.
      core_->builder.CreateRet(ret);

      // validate the generated code, checking for consistency and then return.
      llvm::verifyFunction(*func);
      return func;
    }

    // if any error happens while reading body, remove the function.
    func->eraseFromParent();
    EXCEPTION("Some problem happens while reading body...");
  }
};

} // namespace kaleidoscope17

#endif
