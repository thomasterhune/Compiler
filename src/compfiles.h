
#ifndef compfiles_h
#define compfiles_h

#include <stdio.h>
#include "file_util.h"
#include <string.h>
#include <stdlib.h>

/**
 * \file compfiles.h
 * \brief CompFiles struct and "methods"

    CompFiles is a struct which holds pointers to the compilation input and output files. It also tracks their names and their validation status. It provides methods for prompting the user for valid file names until terminate is requested or all files are validated.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date January 2023
*/


/*
-----------------
CompFiles typedef
-----------------
*/
#pragma region structs
/** Describes the state of a filename validation process */
enum COMPFILES_STATE {
    COMPFILES_STATE_NO_NAME_PROVIDED = 0,
    COMPFILES_STATE_NAME_NEEDS_VALIDATION = 1,
    COMPFILES_STATE_NAME_VALIDATED = 2
};

/**
 * \brief Manages input and output files.
 * 
    CompFiles is a globally accesible struct which maintains references to the loaded files.

    It has a number of functions closely associated to it. In that way it is a class-like, but a singleton. There is only one CompFiles that ever should exist.
*/
struct TCompFiles {
    /*! A file pointer to an open input file. */
    FILE * in;
    /*! A file pointer to an open output file. */
    FILE * out;
    /*! A file pointer to an open tmp file. */
    FILE * temp;
    /*! A file pointer to an open listing file. */
    FILE * listing;
    /*! Determines the status of input file validation. */
    short input_file_state;
    /*! Determines the status of output file validation. */
    short output_file_state;
    /*! Determines the status of listing file validation. */
    short listing_file_state; 
    /*! 1 indicates that a user requested to terminate the program. */
    short terminate_requested;
    /*! The input filename. */
    char * input_file_name;
    /*! The output filename, */
    char * output_file_name;
    /*! The listing filename */
    char * listing_file_name;
    /*! The temp filename */
    char * temp_file_name;
};

/*! The CompFiles singleton. */
struct TCompFiles CompFiles;

#pragma endregion structs

/*
-------------------
CompFiles lifecycle
-------------------
*/
#pragma region lifecycle

/*! Initializes CompFiles struct to default values. 
    \note Covered by unit tests.
*/
void CompFiles_Init();
/*! Closes any open files and returns CompFiles to the default values. Deletes the temp file. */
void CompFiles_DeInit();
/*! 
    Generates a temporary file with a unique name. This file will be destroyed when CompFiles_DeInit() is called. 

    \author klm127
    \date 1/26/2023
*/
void CompFiles_GenerateTempFile();

#pragma endregion lifecycle

/*
-----------------
CompFiles setters
-----------------
*/
#pragma region setters

/*! CompFiles_LoadInputFile loads a new file pointer as the input file. If there is a file already loaded, it closes that file first. 
    \param newInputFile A pointer to an open file in read mode. 
*/
void CompFiles_LoadInputFile(FILE * newInputFile);

/*! CompFiles_LoadOutputFile loads a new file pointer as the output file. If there is a file already loaded, it closes that file first. 
    \param newOutputFile A pointer to an open file in write mode. 
*/
void CompFiles_LoadOutputFile(FILE * newOutputFile);

/*! CompFiles_LoadTempFile loads a new file pointer as the temp file. If there is a file already loaded, it closes that file first. 
    \param newOutputFile A pointer to an open file in write mode. 
*/
void CompFiles_LoadTempFile(FILE * newTempFile);

/*! CompFiles_LoadListingFile loads a new file pointer as the listing file. If there is a file already loaded, it closes that file first. 
    \param newOutputFile A pointer to an open file in write mode. 
*/
void CompFiles_LoadListingFile(FILE * newListingFile);

#pragma endregion setters

/*
------------------
CompFiles prompts
------------------
*/
#pragma region prompts

/*!
    Loops and prompts until all input and output files are set correctly or until terminate is requested.

    \param inputFilename a filename with which to begin input validation with or NULL
    \param outputFilename  a filename with which to begin output validation with or NULL
    \returns 1 if terminate was requested. Otherwise, 0.
    \author klm127
    \date 1/26/2023
*/
short CompFiles_AcquireValidatedFiles(char * inputFilename, const char * outputFilename);


/*!
    Validates an input file name and sets the value in the struct. It will continue looping until the user has supplied a valid filename or elected to quit the program. 

    \param filename a filename with which to begin input validation with or NULL
    \returns 0 if the input file was validated and loaded into the struct. 1 if the user requested to terminate the program. 
*/
short CompFiles_AcquireValidatedInputFile(char * filename);

/*!
    Validates an output file name and sets the value in the struct. It will continue looping until the user has supplied a valid filename or elected to quit the program.  

    \param filename a filename with which to begin input validation with or NULL
    \returns 0 if an output file was validated and loaded into the struct. 1 if the user requested to terminate the program.
*/
short CompFiles_AcquireValidatedOutputFile(const char * filename);

/*!
    Validates a listing file name and sets the value in the struct.

    Called by CompFiles_ValidateOutputFile after an output file has been fully validated. The parameter passed will be the name of the output file with the extension 'list' instead.

    If this file happens to exist, a similar loop will occur as when a user attempts to load an extant output file. The user will be prompted to enter a new file until one is validated or they elect to exit the program. 

    \param filename a filename with which to begin input validation with or NULL
    \returns 0 if an output file was validated and loaded into the struct. 1 if the user requested to terminate the program.
*/
short CompFiles_AcquireValidatedListingFile(const char * filename);

/*!
    Calls the function getString() to recieve a filename from the user and returns it. It will set the 'terminate requested' flag in CompFiles if the user inputs only a \\n. 

    \returns char * inputfilename to be verified

    \author thomaserh99
    \date 1/23/2023
    \note Covered by Unit Tests
*/
char * CompFiles_promptInputFilename();

/*!
    Calls the function getString() to recieve a filename from the user and returns it. It will set the 'terminate requested' flag in CompFiles if the user inputs only a \\n. 

    \warning This should not be called until the input filename has been set. The user may elect to generate an output filename based on the input file. (inputfilename + .out)

    \return A malloced string of an output filename to be verified.

    \author thomaserh99
    \date Created On: 1/23/2023
    \note Covered by Unit Tests
*/
char * CompFiles_promptOutputFilename();

/*!
    Describes the possible selections a user may make when they elect to output to a file that already exists. 
*/
enum USER_OUTPUT_OVERWRITE_SELECTION {
    USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED = 1,
    USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE = 2,
    USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME = 3,
    USER_OUTPUT_TERMINATE_PROGRAM = 4,
    USER_OUTPUT_TERMINATE_INVALID_ENTRY = -1
};
/*!

    Prompts the user as to what they want to do about an output file already existing. It prints a prompt and parses the user response to one of the USER_OUTPUT_OVERWRITE_SELECTION enums. It does NOT loop.

    \returns short corresponding to one of the enums of USER_OTUPUT_OVERWRITE_SELECTION

    \author klm127, thomasterh99, anthony91501
    \date 1/20/2023
    \note Covered by Unit Tests

*/
short CompFiles_promptUserOverwriteSelection();

#pragma endregion prompts

/*
--------------------
CompFiles operations
--------------------
*/
#pragma region operations

/*!
    CompFiles_CopyInputToOutputs copies all the data from the input file to each of the output files. After execution, all output files (tmp, list, and out) will have text identical to the input files. 

    \warning Precondition: All CompFiles file pointers must be open and ready to read/write.

    \author Thomas, Karl
    \date 1/27/2023

    \todo Not Covered by Unit Tests

*/
void CompFiles_CopyInputToOutputs();

#pragma endregion operations


#endif

