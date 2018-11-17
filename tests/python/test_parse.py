import test_env as env
import utils

from os import path
import textwrap


THIS_FILE_PATH = path.abspath(__file__)

expected = textwrap.dedent('''\
    Kaleidoscope >> Parsed a function definition.
    FunctionAST{PrototypeAST{f1()}NumExprAST(5)}
    Kaleidoscope >> Parsed a function definition.
    FunctionAST{PrototypeAST{f2(a, b)}BinaryExprAST(VarExprAST(a) + VarExprAST(b))}
    Kaleidoscope >> Parsed an extern.
    PrototypeAST{h1()}Kaleidoscope >> Parsed an extern.
    PrototypeAST{h2(r)}Kaleidoscope >> Parsed a top-level expr.
    FunctionAST{PrototypeAST{()}BinaryExprAST(VarExprAST(x) + VarExprAST(y))}
    Kaleidoscope >> Parsed a function definition.
    FunctionAST{PrototypeAST{g(v, u)}BinaryExprAST(VarExprAST(v) - CallExprAST{g(VarExprAST(u), NumExprAST(4))})}
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
