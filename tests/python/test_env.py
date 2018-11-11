import argparse
from os import path


PROJECT_NAME = 'kaleidoscope17'

parser = argparse.ArgumentParser(
    prog=f'Tests for {PROJECT_NAME}',
    usage='Run tests. These are intended to be called from ctest in cmake.',
    add_help=True,
)
parser.add_argument('-e',
                    '--target_executable_path',
                    help='path to target executable.',
                    required=True)
parser.add_argument('-b',
                    '--build_dir',
                    help='path to cmake build dir.',
                    required=True)
args = parser.parse_args()

PYTHON_DIR = path.abspath(path.dirname(__file__))
TESTS_DIR = path.abspath((path.join(PYTHON_DIR, '..')))
ROOT_DIR = path.abspath((path.join(TESTS_DIR, '..')))
BUILD_DIR = path.abspath(args.build_dir)
TESTS_DATA_DIR = path.join(TESTS_DIR, 'data')

TARGET_EXECUTABLE_PATH = path.abspath(args.target_executable_path)
