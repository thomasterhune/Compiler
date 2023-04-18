#include "test.h"
#include "CompFiles.h"


void test_CompFiles_Init(CuTest * tc) {
    CompFiles_Init();
    CuAssert(tc, "in, out, temp, and listing files should be null", CompFiles.in == NULL && CompFiles.out == NULL && CompFiles.temp == NULL && CompFiles.listing == NULL);
    CuAssertIntEquals_Msg(tc, "input file state should be no name provided", COMPFILES_STATE_NO_NAME_PROVIDED, CompFiles.input_file_state);
    CuAssertIntEquals_Msg(tc, "output file state should be no name provided", COMPFILES_STATE_NO_NAME_PROVIDED, CompFiles.output_file_state);
    CuAssertIntEquals_Msg(tc, "terminate requested should be 0", 0, CompFiles.terminate_requested);
}

#pragma region TestPrompts

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
    CompFiles_DeInit(1);

}


void test_promptOutputFilename(CuTest *tc) {
    char * test_result;
    CompFiles_Init();

    char * infile = malloc(10 * sizeof(char));
    strcpy(infile, "testext.in");
    CompFiles.input_file_name = infile;

    StdSwapper_Init();
    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = CompFiles_promptOutputFilename();
    CuAssertStrEquals_Msg(tc, "should be testext.out", "testext.out", test_result);
    StdSwapper_RestoreAllStd();
    StdSwapper_DeInit();
    CompFiles_DeInit(1);
}

/*
    test_promptUserOverwriteSelection tests promptUserOverwriteSelection.

    It uses the utility function to redirect stdin and stdout to temporary files in order to test scanf.

                    Authors:    klm127, thomasterh99, anthony91501
                    Created On: 1/20/2023

    Todo: there is too much boilerplate; some of this file closing and removing should be put into setStdin and restoreStdIn. 

*/
void test_promptUserOverwriteSelection(CuTest *tc) {

    short test_result;
    StdSwapper_Init();

    /* If the user enters a "1", it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED*/
    StdSwapper_SetAllStdWithInputOf("1");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '1', it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED", USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);

    /* If the user enters a "n", it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED*/
    StdSwapper_SetAllStdWithInputOf("n");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'n', it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED", USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);

    /* If the user enters a "2", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
    StdSwapper_SetAllStdWithInputOf("2");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '2', it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE", USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);

    /* If the user enters a "o", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
    StdSwapper_SetAllStdWithInputOf("o");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'o', it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE", USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);

    /* If the user enters a "3", it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME*/
    StdSwapper_SetAllStdWithInputOf("3");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '3', it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME", USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);

    /* If the user enters a "\n", it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME*/
    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '\\n', it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME", USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);

    /* If the user enters a "4", it should return USER_OUTPUT_TERMINATE_PROGRAM*/
    StdSwapper_SetAllStdWithInputOf("4");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '4', it should return USER_OUTPUT_TERMINATE_PROGRAM", USER_OUTPUT_TERMINATE_PROGRAM, test_result);

    /* If the user enters a "q", it should return USER_OUTPUT_TERMINATE_PROGRAM*/
    StdSwapper_SetAllStdWithInputOf("q");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'q', it should return USER_OUTPUT_TERMINATE_PROGRAM", USER_OUTPUT_TERMINATE_PROGRAM, test_result);

    /* If the user enters a "8", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    StdSwapper_SetAllStdWithInputOf("8");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '8', it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY", USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);

    /* If the user enters the char "a", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    StdSwapper_SetAllStdWithInputOf("a");
    test_result = CompFiles_promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'a', it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY", USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);

    StdSwapper_DeInit();
}

#pragma region TestPrompts



#pragma region exported

CuSuite* CompfilesGetSuite() {
    CuSuite * suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_CompFiles_Init);
    SUITE_ADD_TEST(suite, test_promptInputFilename);
    SUITE_ADD_TEST(suite, test_promptOutputFilename);
    SUITE_ADD_TEST(suite, test_promptUserOverwriteSelection);
    return suite;
}

#pragma endregion exported