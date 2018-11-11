cmake_minimum_required(VERSION 3.8)
project(kaleidoscope CXX)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-variable -Wno-unused-private-field -fno-omit-frame-pointer -O0 -g")

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(SRC_DIR ${ROOT_DIR}/src)
set(REF_DIR ${ROOT_DIR}/ref)

find_package(LLVM 7 CONFIG)
llvm_map_components_to_libnames(LLVM_LIBS core support demangle mcjit native)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
add_executable(kaleidoscope ${SOURCES})
target_include_directories(kaleidoscope PRIVATE ${INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(kaleidoscope PRIVATE ${LLVM_LIBS})
target_compile_definitions(kaleidoscope PRIVATE ${LLVM_DEFINITIONS})