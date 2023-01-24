
# Tompiler

Tompiler will be a relatively simple compiler built for educational and explorative purposes. 


## Compiling

Compiler configurations are stored in the .bat files. There are two of them.
 - runTests.bat compiles and runs the tests. 
 - compile.bat compiles and runs the code. 

## Folder and file Descriptions

- .vscode : Contains vscode configurations.
- docs : Contains additional documentation
- src : Contains source code
    - file_util.c / .h : file i/o for the compiler
    - main.c : Program entry point
- tests : Contains source code for tests
    - deps: Contains test dependencies
        - CuTest.c / .h : CuTest micro test framework
    - file_util_test.c / .h : tests for file util
    - main_test.c : entry point for test compilation

## Included 3rd party library, CuTest.

[Link to Cutest page](https://cutest.sourceforge.net/)

This is a small bit of code (only 340 lines!) that provides a unit testing skeleton. 

## Credits

- Tom Terhune, ter1023@pennwest.edu
- Karl Miller, mil7865@pennnwest.edu
- Anthony Stepich, ste4864@pennwest.edu

