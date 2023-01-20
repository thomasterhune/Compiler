#include <stdio.h>
#include "file_util_test.h"
#include "file_util.h"

/*
    test_FileExists tests the fileExists function in file_util.c.

    It creates a temporary file and confirms that fileExists returns 1 when checking for that file.

    It checks that fileExists returns 0 when checking for a file that doesn't exist.

    It removes the temporary testing file that was created.

                    Authors:    klm127
                    Created On: 1/19/2023

*/
const char* test_filepath_create = "__temp_test_in.in";
const char* test_filepath_fake = "__temp_test_notHere.in";
void test_FileExists(CuTest *tc) {
    FILE * file = fopen(test_filepath_create, "w");
    fclose(file);

    /* It should return 1 if a file exists. */
    short testResultExists = fileExists(test_filepath_create);
    CuAssertIntEquals(tc, 1, testResultExists);

    /* It should return 0 if a file does not exist. */
    short testResultDoesntExist = fileExists(test_filepath_fake);
    CuAssertIntEquals(tc, 0, testResultDoesntExist);

    remove(test_filepath_create);
}

/*
    test_filenameHasExtension tests the filenameHasExtension function in file_util.c.

    Besides checking basic functionality, it confirms that edge cases such as ending with a '.' and begining with a '.' return the appropriate enumerated negative value. (See header file).


                    Authors:    klm127
                    Created On: 1/19/2023

*/
void test_filenameHasExtension(CuTest *tc) {
    char * test_case;
    int result;

    /* it should be able to find a normal extension. */
    test_case = "myfile.ext";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, 6, result);

    /* it should be able to find an extension even if there are multiple '.'s. */
    test_case = "myfile.ext.ext";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, 10, result);

    /* it should return negative if the string begins with a period. */
    test_case = ".ext";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, FILENAME_STARTS_WITH_PERIOD, result);
    
    /* it should return negative if the string ends with a period. */
    test_case = "ext.";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, FILENAME_ENDS_IN_PERIOD, result);

    /* it should return negative if there is no file extension. */
    test_case = "myfile";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, FILENAME_HAS_NO_PERIOD, result);
}



/* 
    fileUtilGetSuite provides the suite of tests for the file_util module.

    It's used by main_test.c to access the tests within file_util_test; it loads each testing function into the suite and returns it.

    returns CuSuite* - the testing suite for file_util. See CuTest documentation for more information.
*/
CuSuite* fileUtilGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_FileExists);
    SUITE_ADD_TEST(suite, test_filenameHasExtension);
    return suite;
}