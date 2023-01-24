/**
  * 
  * Author: Tom Terhune 
  * 	E-mail: ter1023@pennwest.edu
  * Author: Karl Miller
  * 	E-mail: mil7865@pennnwest.edu
  * Author: Anthony Stepich
  * 	E-mail: ste4864@pennwest.edu
  * 
  * Program 1 - fileopen
  * CSC 460 - Language Translation
  */

#ifndef file_util_h
#define file_util_h


#include <stdbool.h>
#include <stdio.h>

/* 
-------------------------------------------------------------------------------
                Globally Accesible struct(s)                                       
-------------------------------------------------------------------------------
*/
#pragma region structs

/*
    CompFile is a globally accesible struct which maintains references to the loaded files.

    It has a number of functions closely associated to it. In that way it is a class-like, but a singleton. There is only one CompFile that ever should exist.
*/
struct TCompFiles {
    FILE * in;
    FILE * out;
    FILE * temp;
    FILE * listing;
    char * defaultFilename;
};
struct TCompFiles CompFiles;

/* CompFiles_LoadInputFile loads a new file pointer as the input file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadInputFile(FILE * newInputFile);

/* CompFiles_LoadOutputFile loads a new file pointer as the output file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadOutputFile(FILE * newOutputFile);

/* CompFiles_LoadTempFile loads a new file pointer as the temp file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadTempFile(FILE * newTempFile);

/* CompFiles_LoadListingFile loads a new file pointer as the listing file. If there is a file already loaded, it closes that file first. */
void CompFiles_LoadListingFile(FILE * newListingFile);

#pragma endregion structs

/* 
-------------------------------------------------------------------------------
                file operations                                                
-------------------------------------------------------------------------------
*/
#pragma region fileops

/*

    backupFile renames an existing file, adding the extension '.bak' to the end of it. For example 'outFile.out' will become 'outFile.out.bak'.

                    Authors:    klm127
                    Created On: 1/22/2023
                    Covered by Unit Tests
*/
void backupFile(const char * filename);

enum FILE_EXISTS_ENUM {
    FILE_EXISTS = 1,
    FILE_DOES_NOT_EXIST = 0
};
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

#pragma endregion fileops

/* 
-------------------
filename functions                                                
------------------
*/
#pragma region filenames

/*
    The enum FILENAME_EXTENSION_PARSE describes possible return values from filenameHasExtension which indicate different ways which a filename may be invalid.
        no period :         -1
        ends in period:     -2
        starts with period: -3
*/
enum FILENAME_EXTENSION_PARSE {
    FILENAME_HAS_NO_PERIOD = -1,
    FILENAME_ENDS_IN_PERIOD = -2,
    FILENAME_IS_ONLY_PERIOD = -3,
    FILENAME_IS_DIRECTORY = -4
};

/*
    filenameHasExtension checks whether a filename has an extension. It validates that a string would be a valid path but with one additional condition: it must have a period in the file name portion of the path followed by at least one character. 

    parameters:
        - char * filename : the string to check

    returns int:
        - the index of the '.' character in the string if it exists.
        otherwise, one of the negative FILE_EXTENSION_PARSE enums indicating why the filename is invalid;
            (-1) means there was no period.
            (-2) means it ended in a period.
            (-3) means it is only a period.
            (-4) means it ends in a slash and is a directory.

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

/*
    removeExtension modifices the string given in parameters by copying the characters of the string up to the index of the last period.

    PRECONDITION:
        filename has been validated to have a correct extension (not leading with a '.', not ending with a '.')

    parameters:
        char * filename - the char array to modfify

    returns:
        a pointer to a new, extensionless string. This string is allocated with `malloc`. When you are done with it, the memory should be cleared with `free` to avoid memory leaks.

                    Authors:    thomasterh99, klm127
                    Created On: 1/22/2023
                    Covered by Unit Tests

*/
char * removeExtension(const char * filename);



#pragma endregion filenames

/* 
----------------
prompt functions                                                
----------------
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
/*
    function: char * getString()
    
    getString scans a string character by character until recieving a null termination character or a new line

    parameters: none

    returns: a pointer to a new character array given by the user. This string is allocated with `malloc`. When you are done with it, the memory should be cleared with `free` to avoid memory leaks.

                    Authors:    thomaserh99
                    Created On: 1/23/2023
                    NOT Covered by Unit Tests
*/
char * getString();
/*
    function: promptFilename(char * filename)
    
    promptFilename calls the function getString to recieve a filename from the user

    parameters: char * filename - the character array to modify

    returns: a boolean value where false is quit the program when the user enters nothing

                    Authors:    thomaserh99
                    Created On: 1/23/2023
                    NOT Covered by Unit Tests
*/
bool promptFilename(char* filename);


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

/*
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

