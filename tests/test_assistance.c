#include "test_assistance.h"

/* Struct singleton which holds swapped values for std */
struct _TStdReplacer StdR;


/*
    Initializes StdR, setting numbers to negative and objects to NULL.
*/
void StdRInit() {
    StdR.stdin_file_position = -1;
    StdR.saved_stdout = -1;
    StdR.saved_stin = -1;
    StdR.stdout_file_position = -1;
    StdR.temporary_stdin = NULL;
    StdR.temporary_stdout = NULL;
    StdR.temporary_stdin_filename = NULL;
    StdR.temporary_stdout_filename = NULL;
}

FILE * setSTDoutX(char * tempSTDoutFilename) {
    if(StdR.temporary_stdout != NULL) {
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

void restoreSTDoutX() {
    fflush(stdout);
    fclose(StdR.temporary_stdout);
    dup2(StdR.saved_stdout, fileno(stdout));
    clearerr(stdout);
    fsetpos(stdout, &StdR.stdout_file_position);
    remove(StdR.temporary_stdout_filename);
    free(StdR.temporary_stdout_filename);
    StdR.temporary_stdout = NULL;
    StdR.temporary_stdout_filename = NULL;
    StdR.stdout_file_position = -1;
}