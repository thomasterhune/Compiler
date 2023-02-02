#ifndef file_util_h
#define file_util_h
/**
 * \file file_util.h
 * \brief Functions to assist with file operations.
 *
 * \authors Karl Miller, Tom Terhune, Anthony Stepich
 */
#include <stdbool.h>
#include <stdio.h>

/*
----------------
file operations
----------------
*/
#pragma region fileops

/*!
    Renames an existing file, adding the extension '.bak' to the end of it. For example 'outFile.out' will become 'outFile.out.bak'.

    If the backup file exists already, the new file will have additional '.bak's appended until a name is found that does not collide.

    \author klm127
    \date 1/22/2023
    \note Covered by Unit Tests
*/
void backupFile(const char *filename);

/*! Alias for true false, 1, 0 */
enum FILE_EXISTS_ENUM
{
    FILE_CANT_EXIST = -1,
    FILE_EXISTS = 1,
    FILE_DOES_NOT_EXIST = 0
};
/*!
    fileExists checks whether a file with name filename exists.

    \param filename : the filename to check.
    \returns
        short:
        - 1 if the file exists
        - 0 if it does not.
        - -1 if file cant exist
    \authors klm127
    \date 1/19/2023
    \note Covered by Unit Tests

*/
short fileExists(const char *filename);

#pragma endregion fileops

/*
-------------------
filename functions
-------------------
*/
#pragma region filenames

/*!
    The enum FILENAME_EXTENSION_PARSE describes possible return values from filenameHasExtension which indicate different ways which a filename may be invalid.
*/
enum FILENAME_EXTENSION_PARSE
{
    FILENAME_HAS_NO_PERIOD = -1,
    FILENAME_ENDS_IN_PERIOD = -2,
    FILENAME_IS_ONLY_PERIOD = -3,
    FILENAME_IS_DIRECTORY = -4
};

/*!
    filenameHasExtension checks whether a filename has an extension. It validates that a string would be a valid path but with one additional condition: it must have a period in the file name portion of the path followed by at least one character.

    \param filename the string to check

    \returns int:
        - the index of the `.` character in the string if it exists.
        otherwise, one of the negative `FILE_EXTENSION_PARSE` enums indicating why the filename is invalid;
            - (-1) means there was no period.
            - (-2) means it ended in a period.
            - (-3) means it is only a period.
            - (-4) means it ends in a slash and is a directory.

    \author    klm127
    \date 1/19/2023
    \note Covered by Unit Tests

*/
int filenameHasExtension(const char *filename);

/*!
    addExtension modifies the string given by filename by concatenating the string given by extension.

    addExtension returns a pointer to a new, concatenated string. This string is allocated with `malloc`. When you are done with it, the memory should be cleared with `free` to avoid memory leaks.

    \param filename the char array to modify
    \param extension the char array to append

    \authors thomasterh99, klm127
    \date 1/18/2023
    \note Covered by Unit Tests
*/
char *addExtension(const char *filename, const char *extension);

/*!
    removeExtension modifices the string given in parameters by copying the characters of the string up to the index of the last period.

    \pre filename has been validated to have a correct extension (not leading with a '.', not ending with a '.')

    \param filename the filename char* to remove the extension from.

    \return a pointer to a new, extensionless string.

    \warning This string is allocated with `malloc`. When you are done with it, the memory should be cleared with `free` to avoid memory leaks.

    \authors   thomasterh99, klm127
    \date 1/22/2023
    \note Covered by Unit Tests

*/
char *removeExtension(const char *filename);

/*!
    generateAbsolutePath uses a fileapi.h call to generate the absolute path for a given filename.

    \pre filename has already been validated to have an extension
    \param filename the filename to create an absolute path for
    \return a malloced string for a full path name
    \warning ensure the returned string is freed when you are done to avoid memory leaks
    \authors karl, anthony, thomas
    \date 2/1/2023

*/
char *generateAbsolutePath(const char *filename);

/*!
    checkIfSamePaths uses generateAbsolutePath to see if two filenames have the same resulting path.

    \pre both filenames should be validated to be possible filenames.

    \param filename1 the first filename to check.
    \param filename2 the second filename to check.

    \return 1 if they are the same path, 0 otherwise.
    \author karl
    \date 2/1/2023
*/
short checkIfSamePaths(const char *filename1, const char *filename2);

#pragma endregion filenames

/*
---------------------------
prompt assistance functions
---------------------------
*/
#pragma region prompts

/*!
    getString scans a string character by character until recieving a null termination character or a new line

    \return a pointer to a new character array given by the user with a size of the number of characters + 4 for the possible extension
    This string is allocated with `malloc`.
    When you are done with it, the memory should be cleared with `free` to avoid memory leaks.

    \author     thomaserh99
    \date  1/23/2023
    \note Covered by Unit Tests
*/
char *getString();

#pragma endregion prompts

#endif
