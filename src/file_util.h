/**
  * 
  * Author: Tom Terhune
  * Author: Karl Miller
  * Author: Anthony Stepich  
  * 
  * program 1 for CSC 460.
  * 
  */

#ifndef file_util_h
#define file_util_h


#include <stdbool.h>

/* 
-------------------------------------------------------------------------------
                Globally Accesible struct(s)                                       
-------------------------------------------------------------------------------
*/
#pragma region structs

/*
    CompFile is a globally accesible struct which maintains references to the loaded files.
*/
struct TCompFile {
    FILE * in;
    FILE * out;
    FILE * temp;
    FILE * listing;

};
struct TCompFile CompFile;

#pragma endregion structs

/* 
-------------------------------------------------------------------------------
                filename functions                                                
-------------------------------------------------------------------------------
*/
#pragma region filenames

/*
    fileExists checks whether a file given by filename exists.

    parameters:
        - char * filename : the filename to check.
    returns short:
        - 1 if the file exists.
        - 0 if the file does not exist.

                    Authors:    klm127
                    Created On: 1/19/2023
                    Covered by Unit Tests

*/
short fileExists(const char * filename);

/*
    The enum FILENAME_EXTENSION_PARSE describes possible return values from filenameHasExtension which indicate different ways which a filename may be invalid.
        no period :         -1
        ends in period:     -2
        starts with period: -3
*/
enum FILENAME_EXTENSION_PARSE {
    FILENAME_HAS_NO_PERIOD = -1,
    FILENAME_ENDS_IN_PERIOD = -2,
    FILENAME_STARTS_WITH_PERIOD = -3
};

/*
    filenameHasExtension checks whether a filename has an extension. In other words, it checks whether a string has a '.' preceded and followed by at least one character.

    parameters:
        - char * filename : the string to check

    returns int:
        - the index of the '.' character in the string if it exists.
        otherwise, one of the negative FILE_EXTENSION_PARSE enums indicating why the filename is invalid;
            (-1) means there was no period.
            (-2) means it ended in a period.
            (-3) means it started with a period.

                    Authors:    klm127
                    Created On: 1/19/2023
                    Covered by Unit Tests

*/
int filenameHasExtension(const char * filename);


/*
    addExtension modifies the string given by filename by concatenating the string given by extension.

    addExtension returns a pointer to a new, concatenated string. This string is allocated with `malloc`. When you are done with it, the memory should be cleared with `free` to avoid memory leaks.
    
    parameters:
        char * filename - the char array to modify
        char * extension - the char array to append

                    Authors:    thomasterh99, klm127
                    Created On: 1/18/2023
                    Covered by Unit Tests
*/
char * addExtension(const char* filename, const char* extension);

#pragma endregion filenames

/* 
-------------------------------------------------------------------------------
                prompt functions                                                
-------------------------------------------------------------------------------
*/
#pragma region prompts
enum USER_OUTPUT_OVERWRITE_SELECTION {
    USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED = 1,
    USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE = 2,
    USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME = 3,
    USER_OUTPUT_TERMINATE_PROGRAM = 4,
    USER_OUTPUT_TERMINATE_INVALID_ENTRY = -1
};
/*
    promptUserOverwriteSelection prompts the user as to what they want to do about an output file already existing. It prints a prompt and parses the user response to one of the USER_OUTPUT_OVERWRITE_SELECTION enums. It does NOT loop.

    returns short corresponding to one of the enums of USER_OTUPUT_OVERWRITE_SELECTION

                    Authors:    klm127, thomasterh99, anthony91501
                    Created On: 1/20/2023
                    Covered by Unit Tests

*/
short promptUserOverwriteSelection();
#pragma endregion prompts



/**
 * function: bool hasExtension
 * 
 * addExtension modifies the string given by filename by concatenating the string given by extension.
 * 
 * Parameters:
 *   char * filename - the char array to check
 * 
 * Returns:
 *   true if the filename has an extension.
 * 
 * DEPRECATED BY filenameHasExtension
*/
bool hasExtension(char* filename);

/**
 * function: bool promptFilename
 * 
 * // todo: describe function
 * 
 * Parameters:
 *   char * filename - 
 * 
 * Returns:
 *   ...
*/
bool promptFilename(char* inputFilename);

/**
 * function: bool openInputFile
 * 
 * // todo: describe function
 * 
 * Parameters:
 *   char * inputFilename -  
 *   File * inputFile - 
 * 
 * Returns:
 *   ...
*/
bool openInputFile(char* inputFilename, FILE* inputFile);

/**
 * function: void openOutputFile
 * 
 * // todo: describe function
 * 
 * Parameters:
 *   char * outputFilename -  
 *   File * outputFile - 
 * 
 * Returns:
 * 
*/
bool openOutputFile(char* outputFilename, FILE* outputFile);


#endif

