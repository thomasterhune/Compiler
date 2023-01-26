
#ifndef compfiles_h
#define compfiles_h

#include <stdio.h>
#include "file_util.h"
#include <string.h>
#include <stdlib.h>

/*

    CompFiles is a struct which holds...
    
*/


/*
-----------------
CompFies typedef
-----------------
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
    /* The input filename. */
    char * input_file_name;
    /* The output filename, */
    char * output_file_name;
    /* the last filename provided by the user */
    char * lastUserProvidedFilename;
};
struct TCompFiles CompFiles;

#pragma endregion structs

/*
-------------------
CompFiles lifecycle
-------------------
*/
#pragma region lifecycle
/* Initializes CompFiles struct to default values. */
void CompFiles_Init();
/* Closes any open files and returns CompFiles to the default values. */
void CompFiles_DeInit();

#pragma endregion lifecycle

/*
-----------------
CompFiles setters
-----------------
*/
#pragma region setters

/* CompFiles_LoadInputFile loads a new file pointer as the input file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadInputFile(FILE * newInputFile);

/* CompFiles_LoadOutputFile loads a new file pointer as the output file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadOutputFile(FILE * newOutputFile);

/* CompFiles_LoadTempFile loads a new file pointer as the temp file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadTempFile(FILE * newTempFile);

/* CompFiles_LoadListingFile loads a new file pointer as the listing file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadListingFile(FILE * newListingFile);

#pragma endregion setters

/*
--------------------
CompFiles prompts
--------------------
*/
#pragma region prompts

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
    
    Precondition: CompFiles.input_file_name has been set

    promptFilename calls the function getString to recieve an output filename from the user and returns
        the user entered filename or the default filename (inputfilename + .out)

    parameters:


    returns: char * output filename to be verified

                    Authors:    thomaserh99
                    Created On: 1/23/2023
                    Covered by Unit Tests
*/
char * CompFiles_promptOutputFilename();

enum USER_OUTPUT_OVERWRITE_SELECTION {
    USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED = 1,
    USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE = 2,
    USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME = 3,
    USER_OUTPUT_TERMINATE_PROGRAM = 4,
    USER_OUTPUT_TERMINATE_INVALID_ENTRY = -1
};
/*

    CompFiles_promptUserOverwriteSelection prompts the user as to what they want to do about an output file already existing. It prints a prompt and parses the user response to one of the USER_OUTPUT_OVERWRITE_SELECTION enums. It does NOT loop.

    returns short corresponding to one of the enums of USER_OTUPUT_OVERWRITE_SELECTION

                    Authors:    klm127, thomasterh99, anthony91501
                    Created On: 1/20/2023
                    Covered by Unit Tests

*/
short CompFiles_promptUserOverwriteSelection();

#pragma endregion prompts



#endif