import test_env as env
import utils

from os import path
import textwrap


THIS_FILE_PATH = path.abspath(__file__)

expect = textwrap.dedent('''\
Kaleidoscope >> parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{f1()}NumExprAST(5)}

-- generated LLVM IR

define double @f1() {
entry:
  ret double 5.000000e+00
}

Kaleidoscope >> parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{f2(a, b)}BinaryExprAST(VarExprAST(a) + VarExprAST(b))}

-- generated LLVM IR

define double @f2(double %a, double %b) {
entry:
  %add_f = fadd double %a, %b
  ret double %add_f
}

Kaleidoscope >> parsed an extern.

-- parsed result
PrototypeAST{h1()}

-- generated LLVM IR

declare double @h1()

Kaleidoscope >> parsed an extern.

-- parsed result
PrototypeAST{h2(r)}

-- generated LLVM IR

declare double @h2(double)

Kaleidoscope >> parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{g(v, u)}BinaryExprAST(VarExprAST(v) - CallExprAST{g(VarExprAST(u), NumExprAST(4))})}

-- generated LLVM IR

define double @g(double %v, double %u) {
entry:
  %call_f = call double @g(double %u, double 4.000000e+00)
  %sub_f = fsub double %v, %call_f
  ret double %sub_f
}

Kaleidoscope >> parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{f3(x)}BinaryExprAST(BinaryExprAST(NumExprAST(2) + VarExprAST(x)) + BinaryExprAST(VarExprAST(x) + BinaryExprAST(NumExprAST(1) + NumExprAST(2))))}

-- generated LLVM IR

define double @f3(double %x) {
entry:
  %add_f = fadd double 2.000000e+00, %x
  %add_f1 = fadd double %x, 3.000000e+00
  %add_f2 = fadd double %add_f, %add_f1
  ret double %add_f2
}

Kaleidoscope >> parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{f4(z)}BinaryExprAST(BinaryExprAST(BinaryExprAST(NumExprAST(1) + NumExprAST(2)) + VarExprAST(z)) * BinaryExprAST(VarExprAST(z) + BinaryExprAST(NumExprAST(1) + NumExprAST(2))))}

-- generated LLVM IR

define double @f4(double %z) {
entry:
  %add_f = fadd double 3.000000e+00, %z
  %add_f1 = fadd double %z, 3.000000e+00
  %mul_f = fmul double %add_f, %add_f1
  ret double %mul_f
}

Kaleidoscope >> EOF\
''')

if __name__ == "__main__":
    args = [
        f'{env.TARGET_EXECUTABLE_PATH}',  # executable name
        f'{env.TESTS_DATA_DIR}/test_input.txt',  # input data path
        '-debug'  # debug_mode on
    ]
    cmd = ' '.join(args)
    utils.run_test('test_parse_default', cmd, expect)
