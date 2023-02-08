#ifndef tokens_test_h
#define tokens_test_h

#include "CuTest.h"

/*!
    Returns a Cutest testing suite for use with main_test.c.

    Loads all testing functions into a suite using CuTest macros and returns that suite.

    \note When you want to add a test function for something in tokens.c, add the macro call to this function block.
    
    \return A CuSuite to run tests on.
    \date 2/7/2023
    \author klm127
*/
CuSuite* tokensTestGetSuite();

#endif