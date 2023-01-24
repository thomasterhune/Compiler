#ifndef std_swapper_h
#define std_swapper_h
/*
    STD Swapper is a small C utility for swapping out standard input and output to temporary files, and restoring the standard input and output afterwards. Its primary utility may be unit tests, though users may find other applications.

    Author: klm127
    Date: 1/23/2023

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
----------------
typedef and init
----------------
*/
#pragma region inits

/* 
    StdSwapper struct holds the original stdin and stdout values so that they can be replaced when you are done with them. 
*/
typedef struct {
    /* The saved position of stdout. */
    fpos_t stdout_fpos;
    /* The saved reference to the original stdout fileno. */
    int saved_stdout;
    /* The temporary stdout file being written to. */
    FILE * temp_stdout;
    /* The temporary stdout filename. */
    char * temp_stdout_filename;
    /* The default temporary stdout filename. */
    char * default_temp_stdout_filename;

    /* The saved position of stdin. */
    fpos_t stdin_fpos;
    /* The saved reference to the original stdin fileno. */
    int saved_stdin;
    /* The temporary stdin file being written to. */
    FILE * temp_stdin;
    /* The temporary stdin filename. */
    char * temp_stdin_filename;
    /* The default temporary stdin filename. */
    char * default_temp_stdin_filename;
} _Type_StdSwapper;

/* Initializes the swapper. Call it once before using the swapper. */
void StdSwapper_Init();

/* De-initializes the swapper. Closes any active files and deallocs and malloced strings. */
void StdSwapper_DeInit();

#pragma endregion inits

/*
-----------------
STD out swapping
-----------------
*/
#pragma region stdout_funcs

/* Sets stdOut to the given file. The file will be created. */
void StdSwapper_SetStdOut(const char * filename);

/* 
Restores stdOut to the console. (Or whatever it was previously.) 

Parameters:
    - short deleteFile: Whether to remove the temporary file. positive for remove, 0 or negative to keep.
*/
void StdSwapper_RestoreStdOut(short deleteFile);

/* Sets the default filename for convenience methods. */
void StdSwapper_SetDefaultStdoutFile(const char * newDefaultFilename);

#pragma endregion stdout_funcs

/*
---------------
STD in swapping
---------------
*/
#pragma region stdin_funcs

/* Sets stdin to the given file. The file will be created. */
void StdSwapper_SetStdIn(const char * filename);
/* 

Restores stdin to the console. (Or whatever it was previously.) 

Parameters:
    - short deleteFile: Whether to remove the temporary file. positive for remove, 0 or negative to keep.
*/
void StdSwapper_RestoreStdIn(short deleteFile);

/* Sets the default stdin filename for convenience methods. */
void StdSwapper_SetDefaultStdinFile(const char * newDefaultFilename);

#pragma endregion stdin_funcs

/*
-----------------------------------
Combined swapping, convenience calls
-----------------------------------
*/
#pragma region convenience_calls

/*
    Redirects both output and input. Creates the temporary input file and populates it with the string passed as a parameter. 
*/
void StdSwapper_SetAllStdWithInputOf(char * inputForInputFile);

/*
    Restores both output and input. 
*/
void StdSwapper_RestoreAllStd();


#pragma endregion convenience_calls


/*
-------------
Test helpers
------------
*/
#pragma region test_help

/* Gets the swapper. Used for testing purposes; you should let the functions handle managing the values of this struct. */
_Type_StdSwapper* _getSwapper();

#pragma endregion test_help

#endif