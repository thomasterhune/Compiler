#include <stdio.h>
#include "file_util_test.h"
#include "file_util.h"
#include <unistd.h>
#include <stdlib.h>

/* 
------------------------------------------------------------------------------
                Test Filename-related functions                                   
------------------------------------------------------------------------------
*/
#pragma region test_filenames

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

    /* it should return that a filename is invalid if it starts with a period even if it also has a valid extension after*/
    test_case = ".myFile.ext";
    result = filenameHasExtension(test_case);
    CuAssert(tc, "filename starts with a period", result == FILENAME_STARTS_WITH_PERIOD);

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

#pragma endregion test_filenames

/* 
-------------------------------------------------------------------------------
                Test Assistance Utility Functions 
                - STD in/out replacement                         
-------------------------------------------------------------------------------
*/
#pragma region tests_util


struct _TStdReplacer {
    /* The saved position in stdout */
    fpos_t stdout_file_position;
    /* The saved reference to original stdout */
    int saved_stdout;
    /* The saved position in stdin */
    fpos_t stdin_file_position;
    /* The saved reference to original stdin */
    int saved_stin; /* likely only ever to be 0 or 1, but just in case. Actually a pointer */
    /* The temporary stdin being read from while the test is conducted. */
    FILE * temporary_stdin;
    /* The temporary stdout being written to while the test is conducted. */
    FILE * temporary_stdout;
    /* The filename of the temporary file to be deleted when tests are done. */
    char * temporary_stdin_filename;
    /* The filename of the temporary out file to be deleted when tests are done. */
    char * temporary_stdout_filename;
};

/* A singleton which holds the state of STDin replacement. */
struct _TStdReplacer StdR;


/* 
    setSTDout sets the standard output to a new file. This is useful when testing printing functions. Instead of writing to the console, they will write to this new file after this function has been called. 

    Parameters:
        - char * tempSTDoutFilename : the name of the file that should be created which will be written to instead of stdout.
    
    Returns FILE * : a pointer to the new file which was created.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2

*/
FILE * setSTDout(char * tempSTDoutFilename) {
    if(StdR.temporary_stdout != 0) {
        perror("error! setSTDout was called twice in a row!");
    } else {
        fflush(stdout);
        fgetpos(stdout, &StdR.stdout_file_position);
        StdR.saved_stdout = dup(fileno(stdout));
        StdR.temporary_stdout_filename = malloc(sizeof(char) * strlen(tempSTDoutFilename));
        strcpy(StdR.temporary_stdout_filename, tempSTDoutFilename);
        StdR.temporary_stdout = freopen(tempSTDoutFilename, "w", stdout);
    }
    return StdR.temporary_stdout;
}
/*
    restoreSTDout restores standard output to the console. It should be called after all tests on some set of printing functions have been conducted so that output can return to the console.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2
*/
void restoreSTDout() {
    fflush(stdout);
    fclose(StdR.temporary_stdout);
    dup2(StdR.saved_stdout, fileno(stdout));
    clearerr(stdout);
    fsetpos(stdout, &StdR.stdout_file_position);
    StdR.temporary_stdout = 0;
    remove(StdR.temporary_stdout_filename);
    free(StdR.temporary_stdout_filename);
}



/* 
    setSTDin sets the standard input to a new file. This is useful when testing scanning and reading functions. Instead of reading from the console, they will read from this new file after this function has been called. 

    restoreSTDIn() should always be called after you are done redirecting stdin and before calling setSTDin again.

    Parameters:
        - char * tempSTDinFilename : the name of the file that should be created which will be read from instead of stdin.
    
    Returns FILE * : a pointer to the new file which was created.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2

*/
FILE * setSTDin(char * tempSTDinFilename) {
    fflush(stdin);
    fgetpos(stdin, &StdR.stdin_file_position);
    StdR.saved_stin = dup(fileno(stdin));
    StdR.temporary_stdin_filename = malloc(sizeof(char) * strlen(tempSTDinFilename));
    strcpy(StdR.temporary_stdin_filename, tempSTDinFilename);
    StdR.temporary_stdin = freopen(tempSTDinFilename, "r", stdin);
    return StdR.temporary_stdin;
}
/* 
    setSTDin2 prints a string to a file, then sets that file to be the standard input until restoreSTDin() is called.

    It also validates that there is not a temporary stdin already in place.
    
    restoreSTDInHard() should always be called after you are done redirecting stdin and before calling setSTDin again.

    Parameters:
        - char * tempSTDinFilename : the name of the file that should be created which will be read from instead of stdin.
        - char * inputToWirte : a string to write to the file.
    
    Returns FILE * : a pointer to the new file which was created.

                    Authors:    klm127
                    Created On: 1/21/2023
*/
FILE * setSTDin2(char * tempSTDinFilename, char * inputToWrite) {
    FILE * result;
    if(StdR.temporary_stdin != NULL) {
        perror("Restore STDin before attempting to set it again! Call restoreSTDinHard()!");
        result = NULL;
    } else {
        FILE * tempInWriter = fopen(tempSTDinFilename, "w+");
        fputs(inputToWrite, tempInWriter);
        fclose(tempInWriter);
        result = setSTDin(tempSTDinFilename);
    }
    return result;    
}

/*
    restoreSTDin restores standard input to the console / user functions. It should be called after all tests on some set of scanning functions have been conducted so that input can return to the console.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2
*/
void restoreSTDin() {
    fflush(stdin);
    dup2(StdR.saved_stin, fileno(stdin));
    clearerr(stdin);
    fsetpos(stdin, &StdR.stdin_file_position);
}

/*
    restoreSTDinHard restores standard input to console / user functions. It also closes and deletes the temporary input file that was being used. It re-enables setSTDin2.

    Best practice is to use this and setSTDin2 to minimize memory leak risk.

                    Authors:    klm127
                    Created On: 1/21/2023
*/
void restoreSTDinHard() {
    restoreSTDin();
    fclose(StdR.temporary_stdin);
    remove(StdR.temporary_stdin_filename);
    StdR.temporary_stdin = NULL;
    free(StdR.temporary_stdin_filename);
    StdR.temporary_stdin_filename = NULL;
}

/*
    setSTDin3 calls both setSTDin2 and setSTDout. 

    It is the most compact way of redirecting both stds for input tests.

    It creates a file containing the input given by the std_input_text parameter. It also redirects stdout to a dump file. 
    It restoreStd3() should be called after using this.
    
    IMPORTANT: BOTH functions should be called before using any assertions that may change control in order to get your regular std ins and outs back and see the test results!

    parameters:
        char * std_input_text : some text to set to be read next in the standard input (e.g., scanf )
                    Authors:    klm127
                    Created On: 1/21/2023

*/
char * _stdin3_temp_out = "_stdin3_temp_out.txt";
char * _stdin3_temp_in = "_stdin3_temp_in.txt";
void setSTDin3(char * std_input_text) {
    setSTDout(_stdin3_temp_out);
    setSTDin2(_stdin3_temp_in, std_input_text);
}

/*
    restoreSTD3 calls restoreSTDout and restoreSTDin. It sets both back to regular standard input/output. 

    IMPORTANT: BOTH functions should be called before using any assertions that may change control in order to get your regular std ins and outs back and see the test results!
                    Authors:    klm127
                    Created On: 1/21/2023
*/
void restoreSTD3() {
    restoreSTDinHard();
    restoreSTDout();
}

#pragma endregion tests_util


/* 
------------------------------------------------------------------------------
                Test prompts                                                      
------------------------------------------------------------------------------
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

    /* If the user enters a "1", it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED*/
    setSTDin3("1");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);

    /* If the user enters a "2", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
    setSTDin3("2");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);

    /* If the user enters a "3", it should return USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME*/
    setSTDin3("3");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);

    /* If the user enters a "4", it should return USER_OUTPUT_TERMINATE_PROGRAM*/
    setSTDin3("4");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_PROGRAM, test_result);

    /* If the user enters a "9", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    setSTDin3("8");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);

    /* If the user enters the char "a", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
    setSTDin3("a");
    test_result = promptUserOverwriteSelection();
    restoreSTD3();
    CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);
}

#pragma endregion test_prompts

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
    SUITE_ADD_TEST(suite, test_filenameHasExtension);
    SUITE_ADD_TEST(suite, test_promptUserOverwriteSelection);
    SUITE_ADD_TEST(suite, test_addExtension);

    return suite;
}