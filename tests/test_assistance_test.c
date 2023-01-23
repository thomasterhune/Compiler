#include "CuTest.h"
#include "test_assistance.h"


void test_STDswapper(CuTest* tc) {
    /* Ensure it initializes as expected. */
    StdRInit();
    CuAssert(tc, "ints and fpos should be -1", 
        StdR.stdout_file_position == -1 &&
        StdR.saved_stdout == -1 &&
        StdR.stdin_file_position == -1 &&
        StdR.saved_stin == -1
    );
    CuAssert(tc, "obs should be NULL", 
        StdR.temporary_stdin == NULL &&
        StdR.temporary_stdout == NULL &&
        StdR.temporary_stdin_filename == NULL &&
        StdR.temporary_stdout_filename == NULL
    );

    char * temp_out_name = "tests/temp_std_out.txt";
    setSTDoutX(temp_out_name);
    CuAssert(tc, "StdR.temporary_stdout should not be null.", StdR.temporary_stdout != NULL);
    restoreSTDoutX();
    CuAssert(tc, "StdR values should be restored", StdR.temporary_stdout == NULL && StdR.temporary_stdout_filename == NULL && StdR.stdout_file_position == -1);

}

CuSuite* testAssistanceTestsGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_STDswapper);
    return suite;

}