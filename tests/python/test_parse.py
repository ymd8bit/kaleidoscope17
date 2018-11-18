import test_env as env
import utils

from os import path
import textwrap


THIS_FILE_PATH = path.abspath(__file__)

expected = textwrap.dedent('''\
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
    
    Kaleidoscope >> EOF\
''')


if __name__ == "__main__":
    args = [
        f'{env.TARGET_EXECUTABLE_PATH}',  # executable name
        f'{env.TESTS_DATA_DIR}/test_input.txt',  # input data path
        'debug'  # debug_mode on
    ]
    cmd = ' '.join(args)
    result = utils.run_shell(cmd, env.BUILD_DIR)
    assert result == expected, \
        "[test_parse] the result doesn't match to the expected...Test"
