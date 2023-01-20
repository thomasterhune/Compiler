#include <stdio.h>
#include "file_util_test.h"
#include "file_util.h"
#include <unistd.h>

/* 
    -------------------------------------------------------------------------------
    |           Test Assistance Utility Functions                                 |
    -------------------------------------------------------------------------------
*/

/* The saved position in stdout */
fpos_t stdout_file_position;
/* The saved reference to original stdout */
int saved_stdout;

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
    fflush(stdout);
    fgetpos(stdout, &stdout_file_position);
    saved_stdout = dup(fileno(stdout));
    return freopen(tempSTDoutFilename, "w+", stdout);
}
/*
    restoreSTDout restores standard output to the console. It should be called after all tests on some set of printing functions have been conducted so that output can return to the console.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2
*/
void restoreSTDout() {
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    clearerr(stdout);
    fsetpos(stdout, &stdout_file_position);
}

/* The saved position in stdin */
fpos_t stdin_file_position;
/* The saved reference to original stdin */
int saved_stdin;

/* 
    setSTDout sets the standard input to a new file. This is useful when testing scanning and reading functions. Instead of reading from the console, they will read from this new file after this function has been called. 

    Parameters:
        - char * tempSTDinFilename : the name of the file that should be created which will be read from instead of stdin.
    
    Returns FILE * : a pointer to the new file which was created.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2

*/
FILE * setSTDin(char * tempSTDinFilename) {
    fflush(stdin);
    fgetpos(stdin, &stdin_file_position);
    saved_stdin = dup(fileno(stdin));
    return freopen(tempSTDinFilename, "r", stdin);
}

/*
    restoreSTDin restores standard input to the console / user functions. It should be called after all tests on some set of scanning functions have been conducted so that input can return to the console.

                    Authors:    klm127
                    Created On: 1/20/2023

                    Additional credit to StackOverflow answer: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2
*/
void restoreSTDin() {
    fflush(stdin);
    dup2(saved_stdin, fileno(stdin));
    clearerr(stdin);
    fsetpos(stdin, &stdin_file_position);
}

/* 
    -------------------------------------------------------------------------------
    |           Test Filename-related functions                                   |
    -------------------------------------------------------------------------------
*/


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

        /* it should return that a filename is invalid if it starts with a period even if it also has a valid extnesion after*/
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
    -------------------------------------------------------------------------------
    |           Test prompts                                                      |
    -------------------------------------------------------------------------------
*/
    /*
        test_promptUserOverwriteSelection tests promptUserOverwriteSelection.

        It uses the utility function to redirect stdin and stdout to temporary files in order to test scanf.

                        Authors:    klm127
                        Created On: 1/20/2023

        Todo: there is too much boilerplate; some of this file closing and removing should be put into setStdin and restoreStdIn. 

    */
    void test_promptUserOverwriteSelection(CuTest *tc) {
        char * infile = "temp_testIn.txt";
        char * outfile = "temp_testOut.txt";

        FILE * tempOut = setSTDout(outfile);
        FILE * tempIn;
        FILE * tempInWriter;
        short test_result;

        /* If the user enters a "1", it should return USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED*/
        tempInWriter = fopen(infile, "w");
        fputc('1', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);

        /* If the user enters a "2", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
        tempInWriter = fopen(infile, "w");
        fputc('2', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);

        /* If the user enters a "3", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
        tempInWriter = fopen(infile, "w");
        fputc('3', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);

        /* If the user enters a "4", it should return USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE*/
        tempInWriter = fopen(infile, "w");
        fputc('4', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_PROGRAM, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);

        /* If the user enters a "9", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
        tempInWriter = fopen(infile, "w");
        fputc('9', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);

        /* If the user enters a "a", it should return USER_OUTPUT_TERMINATE_INVALID_ENTRY*/
        tempInWriter = fopen(infile, "w");
        fputc('a', tempInWriter);
        fclose(tempInWriter);
        tempIn = setSTDin(infile);
        test_result = promptUserOverwriteSelection();
        CuAssertIntEquals(tc, USER_OUTPUT_TERMINATE_INVALID_ENTRY, test_result);
        restoreSTDin();
        fclose(tempInWriter);
        fclose(tempIn);
        remove(infile);


        restoreSTDout();
        remove(outfile);
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
    SUITE_ADD_TEST(suite, test_promptUserOverwriteSelection);
    return suite;
}