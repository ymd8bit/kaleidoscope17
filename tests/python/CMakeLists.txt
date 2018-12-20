
# python test scripts require some arguments below.
# required arguments:
#   -e TARGET_EXECUTABLE_PATH, --target_executable_path TARGET_EXECUTABLE_PATH
#                         path to target executable.
#   -b BUILD_DIR, --build_dir BUILD_DIR
#                         path to cmake build dir.
message(STATUS ${BUILD_DIR})
message(STATUS ${EXE_PATH})

set(TEST_PARSE_DEFAULT ${TESTS_DIR}/python/test_parse_default.py)
add_test(NAME test_parse_default
         COMMAND python ${TEST_PARSE_DEFAULT} -b ${BUILD_DIR} -e ${EXE_PATH}
         WORKING_DIRECTORY ${TESTS_DIR}/python)

set(TEST_PARSE_PASS_ENABLE ${TESTS_DIR}/python/test_parse_pass_enable.py)
add_test(NAME test_parse_pass_enable
         COMMAND python ${TEST_PARSE_PASS_ENABLE} -b ${BUILD_DIR} -e ${EXE_PATH}
         WORKING_DIRECTORY ${TESTS_DIR}/python)

