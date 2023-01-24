#include <stdio.h>
#include "file_util_test.h"
#include "file_util.h"
#include <stdlib.h>
#include <string.h>
#include "std_swapper.h"

/*
-------------------------------
Test File-op related functions                                   
-------------------------------
*/
#pragma region test_fileops

/*
    test_FileExists tests the fileExists function in file_util.c.

    It creates a temporary file and confirms that fileExists returns 1 when checking for that file.

    It checks that fileExists returns 0 when checking for a file that doesn't exist.

    It removes the temporary testing file that was created.

                    Authors:    klm127
                    Created On: 1/19/2023

*/
const char* test_filepath_create = "tests/__temp_test_in.in";
const char* test_filepath_fake = "tests/__temp_test_notHere.in";
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
    test_backupFile confirms that backupFile() renames an extant file.

                    Authors:    klm127
                    Created On: 1/22/2023
*/
void test_backupFile(CuTest *tc) {
    char * fname = "tests/prevOut.out";
    FILE * file = fopen(fname, "w");
    fclose(file);
    backupFile(fname);
    char * newname = "tests/prevOut.out.bak";
    short exists = fileExists(newname);
    CuAssertIntEquals_Msg(tc, "A file with a .bak extension should exist.", FILE_EXISTS, exists);
    exists = fileExists(fname);
    CuAssertIntEquals_Msg(tc, "The original file should no longer exist.", FILE_DOES_NOT_EXIST, exists);
    remove(newname);
}

#pragma endregion test_fileops

/* 
-------------------------------
Test Filename-related functions                                   
-------------------------------
*/
#pragma region test_filenames


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

    /* it should return that a filename is invalid if it starts with a period even if it also has a valid extension after*/
    test_case = ".myFile.ext";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, "filename can start with a period", 7, result);

    /* it should return negative if the string begins with a period. */
    test_case = ".ext";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, "filename can start with a period", 0, result);
    
    /* it should return negative if the string ends with a period. */
    test_case = "ext.";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, "filename cannot end with a period", FILENAME_ENDS_IN_PERIOD, result);

    /* it should return negative if there is no file extension. */
    test_case = "myfile";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals(tc, FILENAME_HAS_NO_PERIOD, result);

    test_case = "tests/myfile";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check a file in a folder.",
        FILENAME_HAS_NO_PERIOD, result
    );

    test_case = "tests/my.subfolder/out";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to tell that a file in a folder has no extension, even if the folder name has a period.",
        FILENAME_HAS_NO_PERIOD, result
    );

    test_case = "tests\\my.subfolder\\out";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to tell that a file in a folder has no extension, even if the folder name has a period and it uses backslashes.(\\).",
        FILENAME_HAS_NO_PERIOD, result
    );

    test_case = "tests/my.subfolder/out.bak";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to tell that a file in a folder has an extension, even if the folder name has a period.",
        22, result
    );

    test_case = ".vscode/out.hi";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check files within folders which start with an extension.",
        11, result
    );

    test_case = ".vscode/out";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check files within folders which start with an extension.",
        FILENAME_HAS_NO_PERIOD, result
    );

    test_case = "./tests/";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check whether a file is actually a directory.",
        FILENAME_IS_DIRECTORY, result
    );

    test_case = "tests/";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check whether a file is actually a directory.",
        FILENAME_IS_DIRECTORY, result
    );

    test_case = ".vscode/";
    result = filenameHasExtension(test_case);
    CuAssertIntEquals_Msg(tc, 
        "\n\tIt should be able to check whether a file is actually a directory.",
        FILENAME_IS_DIRECTORY, result
    );

}

/*
    test_addExtension tests the addExtension function in file_util.c.

                    Authors:    klm127
                    Created On: 1/19/2023
*/
void test_addExtension(CuTest *tc) {
    char * test_filename;
    char * test_extension;
    char * concatenated;
    test_filename = "mytestfile";
    test_extension = "ext1";
    int test = 0;

    /* it should append the extension to the filename */
    concatenated = addExtension(test_filename, test_extension);
    test = strcmp(concatenated, "mytestfile.ext1");
    CuAssertIntEquals(tc, 0, test);
    free(concatenated);

}

void test_removeExtension(CuTest *tc) {
    char * test_filename;
    char * test_filename_removed_extension;
    int test = 0;
    
    test_filename = "mytestfile.in";

    /*it should remove the extension from the filename*/
    test_filename_removed_extension = removeExtension(test_filename);
    test = strcmp(test_filename_removed_extension, "mytestfile");
    CuAssertIntEquals(tc, 0, test);
    free(test_filename_removed_extension);

    /*it should remove the last extension from the filename*/
    test_filename = "mytestfile.in.in";
    test_filename_removed_extension = removeExtension(test_filename);
    test = strcmp(test_filename_removed_extension, "mytestfile.in");
    CuAssertIntEquals(tc, 0, test);
    free(test_filename_removed_extension);

    /*it should remove the last extension from the filename*/
    test_filename = "m.a";
    test_filename_removed_extension = removeExtension(test_filename);
    test = strcmp(test_filename_removed_extension, "m");
    CuAssertIntEquals(tc, 0, test);
    free(test_filename_removed_extension);

}

#pragma endregion test_filenames

/* 
-------------
Test prompts                                                      
-------------
*/
#pragma region test_prompts
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
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '1', it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED", USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);

    /* If the user enters a "n", it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED*/
    StdSwapper_SetAllStdWithInputOf("n");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'n', it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED", USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);

    /* If the user enters a "2", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
    StdSwapper_SetAllStdWithInputOf("2");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '2', it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE", USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);

    /* If the user enters a "o", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
    StdSwapper_SetAllStdWithInputOf("o");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'o', it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE", USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);

    /* If the user enters a "3", it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME*/
    StdSwapper_SetAllStdWithInputOf("3");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '3', it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME", USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);

    /* If the user enters a "\n", it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME*/
    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '\\n', it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME", USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);

    /* If the user enters a "4", it should return USER_OUTPUT_TERMINATE_PROGRAM*/
    StdSwapper_SetAllStdWithInputOf("4");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '4', it should return USER_OUTPUT_TERMINATE_PROGRAM", USER_OUTPUT_TERMINATE_PROGRAM, test_result);

    /* If the user enters a "q", it should return USER_OUTPUT_TERMINATE_PROGRAM*/
    StdSwapper_SetAllStdWithInputOf("q");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'q', it should return USER_OUTPUT_TERMINATE_PROGRAM", USER_OUTPUT_TERMINATE_PROGRAM, test_result);

    /* If the user enters a "8", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    StdSwapper_SetAllStdWithInputOf("8");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a '8', it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY", USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);

    /* If the user enters the char "a", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    StdSwapper_SetAllStdWithInputOf("a");
    test_result = promptUserOverwriteSelection();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "\n\tIf the user enters a 'a', it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY", USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);

    StdSwapper_DeInit();
}

void test_getString(CuTest *tc) {
    char* test_result;
    printf("testing");

    /* If the user enters a "\n", it should return previous characters*/
    StdSwapper_Init();

    StdSwapper_SetAllStdWithInputOf("hello.txt\n");
    test_result = getString();
    StdSwapper_RestoreAllStd();

    StdSwapper_DeInit();
    CuAssertStrEquals_Msg(tc, "\n\tIf the user enters a 'hello.txt', it should return hello.txt", "hello.txt", test_result);

}

void test_promptFilename(CuTest *tc) {
    char * test_result;
    CompFiles_Init();
    StdSwapper_Init();

    StdSwapper_SetAllStdWithInputOf("nada.txt\n");
    test_result = promptFilename();
    StdSwapper_RestoreAllStd();
    CuAssertStrEquals_Msg(tc, "Our string should be the result of the user input.", "nada.txt", test_result);

    StdSwapper_SetAllStdWithInputOf("\n");
    test_result = promptFilename();
    StdSwapper_RestoreAllStd();
    CuAssertIntEquals_Msg(tc, "Our string should be the result of the user input.", CompFiles.terminate_requested, 1);

    StdSwapper_DeInit();
    CompFiles_DeInit();

}

#pragma endregion test_prompts

/* 
-------------
Sanity Tests                                                     
-------------
*/
#pragma region test_sanity

void test_getc(CuTest *tc) {
    char result;
    printf("\nLet's test getc.\n\tPress 'Enter'!\n");
    result = getchar();
    CuAssertIntEquals_Msg(tc, "The result should be '\n'.", '\n', result);
}



#pragma endregion test_sanity

/* 
    fileUtilGetSuite provides the suite of tests for the file_util module.

    It's used by main_test.c to access the tests within file_util_test; it loads each testing function into the suite and returns it.

    Important:

    Whenever you add a testing function for file_util, you must add a call to SUITE_ADD_TEST inside this function to load the test into the file_util suite.

    returns CuSuite* - the testing suite for file_util. See CuTest documentation for more information.
*/
CuSuite* fileUtilGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_FileExists);
    SUITE_ADD_TEST(suite, test_backupFile);
    SUITE_ADD_TEST(suite, test_filenameHasExtension);
    SUITE_ADD_TEST(suite, test_removeExtension);
    /* SUITE_ADD_TEST(suite, test_directory_validation); <--- a sanity test only */
    /* SUITE_ADD_TEST(suite, test_getc);  <--- a sanity test only */
    SUITE_ADD_TEST(suite, test_promptUserOverwriteSelection); 
    SUITE_ADD_TEST(suite, test_getString);
    SUITE_ADD_TEST(suite, test_promptFilename);
    SUITE_ADD_TEST(suite, test_addExtension);
    return suite;
}