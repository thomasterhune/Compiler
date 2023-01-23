#ifndef test_assistance_tests_h
#define test_assistance_tests_h

/*
    test_assistance_test tests the assistance functions for the testing framework.

    The only thing it needs to export is testAssistanceTestsGetSuite. This file should not have to be changed. 
*/

#include "CuTest.h"
#include "test_assistance.h"

/*
    testAssistanceTestsGetSuite returns the test suite for testing the test-assistance suites.
*/
CuSuite* testAssistanceTestsGetSuite();

#endif