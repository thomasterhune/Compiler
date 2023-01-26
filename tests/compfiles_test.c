#include "compfiles_test.h"


void test_CompFiles_Init(CuTest * tc) {
    CompFiles_Init();
    CuAssert(tc, "in, out, temp, and listing files should be null", CompFiles.in == NULL && CompFiles.out == NULL && CompFiles.temp == NULL && CompFiles.listing == NULL);
    CuAssertIntEquals_Msg(tc, "input file state should be no name provided", COMPFILES_STATE_NO_NAME_PROVIDED, CompFiles.input_file_state);
    CuAssertIntEquals_Msg(tc, "output file state should be no name provided", COMPFILES_STATE_NO_NAME_PROVIDED, CompFiles.output_file_state);
    CuAssertIntEquals_Msg(tc, "terminate requested should be 0", 0, CompFiles.terminate_requested);
}

void test_promptInputFilename(CuTest *tc) {
    char * test_result;
    CompFiles_Init();
    StdSwapper_Init();

    StdSwapper_SetAllStdWithInputOf("nada.txt\n");
    test_result = CompFiles_promptInputFilename();
    StdSwapper_RestoreAllStd();
    CuAssertStrEquals_Msg(tc, "Our string should be the result of the user input.", "nada.txt", test_result);

    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = CompFiles_promptInputFilename();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "Our string should be the result of the user input.", CompFiles.terminate_requested, 1);

    StdSwapper_DeInit();
    CompFiles_DeInit();

}


void test_promptOutputFilename(CuTest *tc) {
    char * test_result;
    CompFiles_Init();
    StdSwapper_Init();

    StdSwapper_SetAllStdWithInputOf("nada.txt\n");
    test_result = CompFiles_promptOutputFilename();
    StdSwapper_RestoreAllStd();
    CuAssertStrEquals_Msg(tc, "Our string should be the result of the user input.", "nada.txt", test_result);

    CompFiles.input_file_name = "test";
    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = CompFiles_promptOutputFilename();
    StdSwapper_RestoreAllStd();
    CuAssertStrEquals_Msg(tc, "Our string should be the result of the user input.", "test.out", test_result);

    StdSwapper_DeInit();
    CompFiles_DeInit();

}



#pragma region exported

CuSuite* CompfilesGetSuite() {
    CuSuite * suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_CompFiles_Init);
    SUITE_ADD_TEST(suite, test_promptInputFilename);
    /*SUITE_ADD_TEST(suite, test_promptOutputFilename);*/
    return suite;
}

#pragma endregion exported