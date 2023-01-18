#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef file_util_h
#define file_util_h
#endif

/**
 * function: void addExtension
 * 
 * addExtension modifies the string given by filename by concatenating the string given by extension.
 * 
 * Parameters:
 *   char * filename - the char array to modify
 *   char * extension - the char array to append
*/
void addExtension(char* filename, char* extension);

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
*/
void openOutputFile(char* outputFilename, FILE* outputFile);

