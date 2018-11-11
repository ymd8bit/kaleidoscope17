import test_env as env
import utils

from os import path
import textwrap


THIS_FILE_PATH = path.abspath(__file__)

expected = textwrap.dedent('''\
    Kaleidoscope >> TokenType::Def
    ID(f)
    TokenType::ParenL
    ID(a)
    TokenType::Comma
    ID(b)
    TokenType::ParenR
    ID(a)
    TokenType::GenericAscii(+)
    ID(b)
    TokenType::Semicolon
    Parsed a function definition.
    Kaleidoscope >> ID(x)
    TokenType::GenericAscii(+)
    ID(y)
    TokenType::Semicolon
    Parsed a top-level expr.
    Kaleidoscope >> TokenType::Def
    ID(g)
    TokenType::ParenL
    ID(v)
    TokenType::Comma
    ID(u)
    TokenType::ParenR
    ID(v)
    TokenType::GenericAscii(-)
    ID(g)
    TokenType::ParenL
    ID(u)
    TokenType::Comma
    Number,(4.000000)
    TokenType::ParenR
    TokenType::Semicolon
    Parsed a function definition.
    Kaleidoscope >> TokenType::EOF
    EOF\
''')


if __name__ == "__main__":
    cmd = f'{env.TARGET_EXECUTABLE_PATH} {env.TESTS_DATA_DIR}/test_input.txt'
    result = utils.run_shell(cmd, env.BUILD_DIR)
    assert result == expected, \
        "[test_parse] the result doesn't match to the expected...Test"
