
#ifndef compfiles_h
#define compfiles_h

#include <stdio.h>
#include "file_util.h"
#include <string.h>
#include <stdlib.h>

/*

    CompFiles is a struct which holds...
    
*/


#pragma region structs
/* Describing the states of the machine */
enum COMPFILES_STATE {
    COMPFILES_STATE_NO_NAME_PROVIDED = 0,
    COMPFILES_STATE_NAME_NEEDS_VALIDATION = 1,
    COMPFILES_STATE_NAME_VALIDATED = 2
};

/*
    CompFile is a globally accesible struct which maintains references to the loaded files.

    It has a number of functions closely associated to it. In that way it is a class-like, but a singleton. There is only one CompFile that ever should exist.
*/
struct TCompFiles {
    /* A file pointer to an open input file. */
    FILE * in;
    /* A file pointer to an open output file. */
    FILE * out;
    /* A file pointer to an open tmp file. */
    FILE * temp;
    /* A file pointer to an open listing file. */
    FILE * listing;
    /* Determines the status of input file validation. */
    short input_file_state;
    /* Determines the status of output file validation. */
    short output_file_state;
    /* 1 indicates that a user requested to terminate the program. */
    short terminate_requested;
    /* The default filename. */
    char * input_file_name;
    /* the last filename provided by the user */
    char * lastUserProvidedFilename;
};
struct TCompFiles CompFiles;

/* 

    Initializes CompFiles struct to default values. 

*/
void CompFiles_Init();


void CompFiles_DeInit();

/* 
    Validates an input file name and sets the value in the struct.

    Returns:
        0 if an input file was validated and loaded into the struct.
        1 if the user requested to terminate the program. 
*/
short CompFiles_ValidateInputFile(const char * filename);

/*
    Validates an output file name and sets the value in the struct.

    Returns:
        0 if an output file was validated and loaded into the struct.
        1 if the user requested to terminate the program.
*/
short CompFiles_ValidateOutputFile(const char * filename);



/* CompFiles_LoadInputFile loads a new file pointer as the input file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadInputFile(FILE * newInputFile);

/* CompFiles_LoadOutputFile loads a new file pointer as the output file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadOutputFile(FILE * newOutputFile);

/* CompFiles_LoadTempFile loads a new file pointer as the temp file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadTempFile(FILE * newTempFile);

/* CompFiles_LoadListingFile loads a new file pointer as the listing file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadListingFile(FILE * newListingFile);


/*
    function: CompFiles_promptInputFilename()
    
    promptFilename calls the function getString to recieve a filename from the user and returns it

    parameters:

    mutates: Will set the 'terminate requested' flag in CompFiles if the user inputs only a \\n. 

    returns: char * inputfilename to be verified

                    Authors:    thomaserh99
                    Created On: 1/23/2023
                    Covered by Unit Tests
*/
char * CompFiles_promptInputFilename();

/*
    function: CompFiles_promptOutputFilename()
    
    promptFilename calls the function getString to recieve an output filename from the user and returns
        the user entered filename or the default filename (inputfilename + .out)

    parameters:


    returns: char * output filename to be verified

                    Authors:    thomaserh99
                    Created On: 1/23/2023
                    NOT Covered by Unit Tests
*/
char * CompFiles_promptOutputFilename();



#endif