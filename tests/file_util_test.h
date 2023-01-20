#ifndef file_util_test_h
#define file_util_test_h

#include "CuTest.h"

/*
    There is unlikely to be any reason to change this file in the future. It only needs to export the single function in order to get the testing suite to main_test. Any other unit tests should be written as non-exported members in `file_util_test.c`.

    Then they are loaded in the file_util_test suite within fileUtilGetSuite


*/


/*
    fileUtilGetSuite returns a Cutest testing suite for use with main_test.c.

    This function loads all the testing functions for the functions defined in file_util into the suite.

    - Returns: A pointer to a CuSuite.
*/
CuSuite* fileUtilGetSuite();

#endif