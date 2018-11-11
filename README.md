# kaleidoscope17
Kaleidoscope17 is an implementation of Kaleidoscope in LLVM tutorial.
https://llvm.org/docs/tutorial/index.html
This project tries to rewrite the original Kaleidoscope with modern C++(17).
Now, this project only covers until chapter "[2. Kaleidoscope: Implementing a Parser and AST](
https://llvm.org/docs/tutorial/LangImpl02.html)".

## Requirements
- LLVM (>= 7.0)
- cmake (>= 3.8)
- python (>= 3.6)


## Build
We can build this project with cmake. 
Here is an example.
```shell
mkdir build
cd build

# this example uses Ninja as the generator but of course
# you can choose normal "Unix Makefiles" instead.
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH={path-to-install-llvm} ..
ninja all

# run tests this will call python script inside
ctest
```

This project is going to require LLVM (not yet), therefore you need to specify the installed directory by setting `CMAKE_PREFIX_PATH` to cmake.
if you need the detailed, please check the official guide.
https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project