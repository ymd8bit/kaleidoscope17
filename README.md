# kaleidoscope17
Kaleidoscope17 is an implementation of [Kaleidoscope in LLVM tutorial](https://llvm.org/docs/tutorial/index.html).   
This project tries to rewrite the original Kaleidoscope with modern C++17.  
Now, this project only covers until chapter "[3. Kaleidoscope: Code generation to LLVM IR](
https://llvm.org/docs/tutorial/LangImpl03.html)".  
So, this currently cannot execute given codes.

## Requirements
- LLVM (>= 7.0)
- cmake (>= 3.8)
- python (>= 3.6)

## Usage
kaleidoscope17 provides two types of interface: interpreter, source code file.  

### Interpreter
You just can access the interpreter interface by running the binary without any argument.  
The following example shows a case when we define an add function there.  
```
>> ./kaleidoscope17
>> Kaleidoscope >> def f(a, b) a + b;
parsed a function definition.

-- parsed result
FunctionAST{PrototypeAST{f(a, b)}BinaryExprAST(VarExprAST(a) + VarExprAST(b))}

-- generated LLVM IR

define double @f(double %a, double %b) {
entry:
  %add_f = fadd double %a, %b
  ret double %add_f
}
```

### Source code file
You also can use this compiler with your source code files.  
If you take the keyword "debug" additionally, you can see the parsed result and the generated LLVM IR from your code.
```
# the given line in this source code is
# def g(v, u) v - g(u, 4.0);

-- parsed result
FunctionAST{PrototypeAST{g(v, u)}BinaryExprAST(VarExprAST(v) - CallExprAST{g(VarExprAST(u), NumExprAST(4))})}

-- generated LLVM IR

define double @g(double %v, double %u) {
entry:
  %call_f = call double @g(double %u, double 4.000000e+00)
  %sub_f = fsub double %v, %call_f
  ret double %sub_f
}
```


## Build
You can build this project with cmake. 
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
if you need the detailed, please check [the official guide](https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project).
