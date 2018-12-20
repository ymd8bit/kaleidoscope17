cmake_minimum_required(VERSION 3.8)
project(kaleidoscope CXX)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wpedantic -Wno-unused-variable -Wno-unused-private-field -Wno-error=unused-command-line-argument -fno-omit-frame-pointer -fsanitize=address -O0 -g")


get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)
get_filename_component(INCLUDE_DIR ${ROOT_DIR}/include ABSOLUTE)
get_filename_component(SRC_DIR ${ROOT_DIR}/src ABSOLUTE)
get_filename_component(TESTS_DIR ${ROOT_DIR}/tests ABSOLUTE)
get_filename_component(BUILD_DIR ${CMAKE_BINARY_DIR} ABSOLUTE)

find_package(LLVM 7 CONFIG)
# llvm_map_components_to_libnames(LLVM_LIBS core support demangle mcjit native)
llvm_map_components_to_libnames(LLVM_LIBS all)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")

# apply clang-tidy
find_program(
  CLANG_TIDY_FOUND
  NAMES "clang-tidy"
  PATHS "${LLVM_DIR}"
  PATH_SUFFIXES "../../bin"
  DOC "Path to clang-tidy executable"
)

# if(NOT CLANG_TIDY_FOUND)
#   message(STATUS "clang-tidy is not found.")
# else()
#   set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_FOUND}; -checks=*,google-readability-casting,-clang-analyzer-alpha.*,-header-filter='.*'")
#   # message(STATUS "clang-tidy is found: ${CLANG_TIDY_FOUND}")
#   # set(CLANG_TIDY_CMD "${CLANG_TIDY_FOUND} -checks=*,google-readability-casting,-clang-analyzer-alpha.*,-header-filter=${INCLUDE_DIR}")
#   # message(STATUS "clang-tidy command: ${CLANG_TIDY_CMD}")
# endif()


file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

set(EXE_NAME kaleidoscope17)
add_executable(${EXE_NAME} ${SOURCES})
target_include_directories(${EXE_NAME} PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(${EXE_NAME} PRIVATE ${LLVM_LIBS})
target_compile_definitions(${EXE_NAME} PRIVATE ${LLVM_DEFINITIONS})
set_property(GLOBAL
  PROPERTY CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
)

set(EXE_PATH ${BUILD_DIR}/${EXE_NAME})

enable_testing()
add_subdirectory(${TESTS_DIR})