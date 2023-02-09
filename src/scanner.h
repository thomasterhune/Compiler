#ifndef scanner_h
#define scanner_h

#include <stdio.h>

/* The initial buffer size for the scanner's buffer. It will be expanded when necessary. */
#define TSCANNER_INIT_BUFF_SIZE 100

/*!
    \file scanner.h
    \brief Scanner struct and 'method'

    We deviated slightly from the suggested implementation in that we are maintaining a scanner singleton that operates on its members as 'methods'.

    The scanner is responsible for reading an input file and extracting tokens. See tokens.h / tokens.c for more information.

*/

#pragma region structs
/*!
    TScanner holds the static data a scanner needs to process files.
*/
struct TScanner{
    /*A character array for the input buffer*/
    char * buffer;
    /*! The current buffer size, in bytes. */
    int current_buff_size; 
    /*! The current position being read in the buffer.*/
    int buf_pos;
    /*! A running counter of the line being read. */
    int line_count;
    /*! A running counter of the column being read. */
    int col_count;
    /*! Character array to hold token*/
    char * token;
    /*! Short referencing error flag*/
    short errorFlag;
    /*! A file pointer to an open input file. */
    FILE *in;
    /*! A file pointer to an open output file. */
    FILE *out;
    /*! A file pointer to an open tmp file. */
    FILE *temp;
    /*! A file pointer to an open listing file. */
    FILE *listing;

};

/*! The singleton instance of the scanner. */
struct TScanner scanner;

#pragma endregion structs

#pragma region lifecycle
/*!
    Initializes the scanner.

    \pre Each of the files is open and ready for reading or writing.
    \param in input file for reading
    \param out output file for output
    \param listing listing file output for scanner process
    \param temp temporary file 
    
*/
void Scanner_Init(FILE * in, FILE * out, FILE * listing, FILE * temp);

/*!
    De-Initializes the scanner. Frees the buffer string. Forgets the file pointer references. Scanner_Init() should be called again if the scanner is to be re-used. 
*/
void Scanner_DeInit();

#pragma endregion lifecycle

#pragma region buffer

/*! 
    Clears scanner.buffer and sets it to a new buffer of default size by calling refreshBuffer in scanner_util. 
*/
void Scanner_clearBuffer();

/*! 
    Resizes the scanners buffer to be twice as large as it currently is, maintaining all data already within it.
*/
void Scanner_expandBuffer();


#pragma endregion buffer

/*!
Scanner_addChar add a char to the buffer

*/
void Scanner_addChar();


#pragma region logic

void Scanner_Scan();

#pragma endregion logic

#endif