#include "CuTest.h"
#include "test.h"
#include <stdio.h>

/*
    This file contains essentially boilerplate code as described in `docs/CuTest-Readme.txt`. 

    It loads all relevant test suites and runs the tests. 

    There is a separate compilation pipeline for the testing executable. In VSCode, it is set up to run via the Tests task.

*/

/*
    Loads all testing suites, runs the unit tests, and prints the output.
*/
void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite * suite = CuSuiteNew();

    CuSuiteAddSuite(suite, fileUtilGetSuite());
    CuSuiteAddSuite(suite, CompfilesGetSuite());
    CuSuiteAddSuite(suite, tokensTestGetSuite());
    CuSuiteAddSuite(suite, dfaTestGetSuite());
 
    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output -> buffer);
}

int main(void) {
    RunAllTests();
}