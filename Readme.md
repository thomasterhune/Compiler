
# Tompiler Readme

Tompiler will be a relatively simple compiler built for educational and explorative purposes. 

## Compiling

Compiler configurations are stored in the .bat files. There are two of them.
 - runTests.bat compiles and runs the tests. 
 - compile.bat compiles and runs the code. 

## Using

Running `compile.bat` will run the compiler after executing. You can also find the executable, `fileopen.exe`, in your `bin` directory.

It takes up to two command line arguments. The first argument can be an input file path while the second argument can be an output file path.

Place the `bin` directory on your system path if you want to be able to run tompiler from anywhere. 
## Folder and file Descriptions

- .vscode : Contains vscode configurations.
- docs : Contains additional documentation
- src : Contains source code
    - main.c : Program entry point
    - compfiles.c / .h : struct for managing input output file access
    - file_util.c / .h : file i/o helpersfor the compiler
    - dfa.c / .h : The DFA which drives the scanning process.
    - 
- tests : Contains source code for tests
    - lib: Contains test dependencies
        - CuTest.c / .h : CuTest micro test framework
        - std_swapper.c / .h : For swapping stdin and out with files.
    - file_util_test.c  : tests for file util
    - dfa_test.c : tests for dfa.
    - tokens_test.c : test for token functions.
    - main_test.c : entry point for test compilation
    - tests.h : each test file has one exported member, a function that returns the testing suite. They are all declared here.

## Included 3rd party library, CuTest.

[Link to Cutest page](https://cutest.sourceforge.net/)

This is a small bit of code (only 340 lines!) that provides a unit testing skeleton. 

## Credits

- Tom Terhune
- Karl Miller
- Anthony Stepich

