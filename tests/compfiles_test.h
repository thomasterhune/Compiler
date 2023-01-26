#ifndef compfiles_test_h
#define compfiles_test_h

#include "CuTest.h"
#include "compfiles.h"

/*
    compfilesGetSuite returns a Cutest testing suite for use with main_test.c.

    This function loads all the testing functions for the functions defined in compfiles_test into the suite.

    - Returns: A pointer to a CuSuite.
*/
CuSuite* CompfilesGetSuite();

#endif 