
# python test scripts require some arguments below.
# required arguments:
#   -e TARGET_EXECUTABLE_PATH, --target_executable_path TARGET_EXECUTABLE_PATH
#                         path to target executable.
#   -b BUILD_DIR, --build_dir BUILD_DIR
#                         path to cmake build dir.
set(TEST_PARSE_PY ${TESTS_DIR}/python/test_parse.py)
message(STATUS ${BUILD_DIR})
message(STATUS ${EXE_PATH})
add_test(NAME test_parse
         COMMAND python ${TEST_PARSE_PY} -b ${BUILD_DIR} -e ${EXE_PATH}
         WORKING_DIRECTORY ${TESTS_DIR}/python)
