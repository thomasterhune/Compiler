#ifndef test_assistance_h
#define test_assistance_h
/*
    test_assistance provides helper functions for conducting unit tests.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
-------------------
STD in/out swapping
-------------------
*/
#pragma region std_swap
/* Struct singleton which holds swapped values for std */
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
struct _TStdReplacer StdR;

void StdRInit();
FILE * setSTDOutX(char * temporarySTDoutFilename);
void restoreSTDoutX();

#pragma endregion std_swap

#endif

