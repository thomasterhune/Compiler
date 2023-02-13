#ifndef scanner_h
#define scanner_h

#define SCANNER_PRINTS_TO_CONSOLE 1

#include <stdio.h>

#include "tokens.h"

/* The initial buffer size for the scanner's buffer. It will be expanded when necessary. */
#define TSCANNER_INIT_BUFF_SIZE 100

/*!
    \file scanner.h
    \brief Scanner struct and 'method'

    We achieve scanning using a singleton struct called scanner of type TScanner that operates on its members using 'methods'. Every scanner method (every function in this file) begins with 'Scanner_' to indicate that it will be mutating the scanner singleton. 

    Scanner reads from the input file one line at a time. That is, it reads a line until it hits either a newline or EOF. This line is read into an expanding malloced buffer to allow for excessively large line sizes, though it prefers line sizes under 100 characters. 

    The scanner methods call the generic functions in scanner_util to read its buffer.

    The scanner is responsible for reading an input file and extracting tokens. See tokens.h / tokens.c for more information.

*/

/*
-------------------
Scanner struct/typedef
-------------------
*/
#pragma region structs
/*!
    TScanner holds the static data a scanner needs to process files.
*/
typedef struct {
    /*A character array for the input buffer*/
    char * buffer;
    /*! The current buffer size, in bytes. */
    int current_buff_size; 
    /*! The current position being read in the buffer and a running counter of the column being read.*/
    int buf_pos;
    /*! A running counter of the line being read. */
    int line_count;
    /* An array of boundry characters that delimit literals, keywords, and identifiers. Effectively all operator characters plus space, newline, and EOF. Set in Scanner_Init().*/
    char * boundaries; 
    /* Set in populateBuffer when a line ends in the EOF char. */
    short on_last_line;
    /*! A file pointer to an open input file. */
    FILE *in;
    /*! A file pointer to an open output file. */
    FILE *out;
    /*! A file pointer to an open tmp file. */
    FILE *temp;
    /*! A file pointer to an open listing file. */
    FILE *listing;
    /*! The buffer of found tokens that will be read by the parser. */
    TokenCatch* tokens;
    /*! The length of the tokens array. */
    int l_tokens;

} TScanner;

/*! The singleton instance of the scanner. */
TScanner scanner;

#pragma endregion structs
/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle
/*!
    Initializes the scanner.
    
*/
void Scanner_Init();

/*!
    De-Initializes the scanner. Frees the buffer string. Forgets the file pointer references. Scanner_Init() should be called again if the scanner is to be re-used. 
*/
void Scanner_DeInit();

#pragma endregion lifecycle
/*
-------------------
Scanner buffer
-------------------
*/
#pragma region buffer
/*! 
    Clears scanner.buffer and sets it to a new buffer of default size by calling refreshBuffer in scanner_util. 
*/
void Scanner_clearBuffer();

/*! 
    Resizes the scanners buffer to be twice as large as it currently is, maintaining all data already within it.
*/
void Scanner_expandBuffer();

/*!
    Reads a file until a newline character or EOF character is found. Puts all found characters in its buffer. Expands the buffer if necessary to fit a line.

    It also increments scanner's line counter and resets buf_pos (which doubles as a column counter) to 0.

    It also calls Scanner_printLine() to print the current line to the listing file. 
    
    \returns The number of characters read into the buffer.
    \author klm127
    \date 2/10/2023
*/
int Scanner_populateBuffer();

#pragma endregion buffer

/*
----------------
Scanner logic
----------------
*/
#pragma region logic
/*!
    Executes calls to other Scanner methods to perform the scanning logic. Uses Scanner_Scan to look ahead and determine the correct function to call with a switch statement. Effectively drives the scanning process.

    \pre Each of the files is open and ready for reading or writing.
    \param in input file for reading
    \param out output file for output
    \param listing listing file output for scanner process
    \param temp temporary file 
*/
void Scanner_Scan(FILE * in, FILE * out, FILE * listing, FILE * temp);
/*!
    Peeks ahead in the scanner. Starts by looking at current position and scans forward if a '-' is found so it can identify a comment. It does not move the buf pos.
    \returns A SCAN_LHEAD_VAL corresponding to what it found. SCAN_LHEAD_WHITESPACE=0, SCAN_LHEAD_EOF=1, SCAN_LHEAD_NLINE=2, SCAN_LHEAD_COMMENT=3, SCAN_LHEAD_OPERATOR=4, SCAN_LHEAD_NUMBER=5, SCAN_LHEAD_WORD=6
    \author klm127
    \date 2/10/2023
*/
short Scanner_lookAhead();

/*!
    Scanner_takeAction evaluates the result of calling Scanner_lookAhead() and calls the appropriate method using a switch statement. 
*/
void Scanner_takeAction(short lookAheadResult);

#pragma endregion logic

/*
---------------
Scanner actions
---------------
*/
#pragma region actions
/*!
    Moves the scanner buffer pointer to the first non-whitespace found from its current position.
*/
void Scanner_Skipwhitespace();


/*!
    Moves the buffer index forward until a whitespace or an operator. Returns everything between the start position and the end position as a new, malloced char string.

    \returns A malloced char string. 
*/
char* Scanner_ExtractWord();

/*!
    Moves the buffer index forward until a non-number is found. Returns everything between the start position and the end position as a new, malloced char string.

    \returns A malloced char string consisting only of digits.
*/
char* Scanner_ExtractInteger();

/*!
    Scanner_ExtractOperator is called by dispatch action when the lookahead is an operator. 

    \returns A int corresponding to a token. 
*/
int Scanner_ExtractOperator();

/*!

*/
int extractOperator(char * buffer, int * index);

#pragma endregion actions

/*
----------------
Scanner printing
----------------
*/
#pragma region printing

/*!
    Prints the line number and a tab, then prints the current buffer to the listing file, starting at position 0 and ending at scanner.buf_pos.

    It will also print to stdout if the flag is set in the header file.

    \author klm127
    \date 2/12/2023
*/
void Scanner_printLine();

#pragma endregion printing
/*
----------------
Scanner debug
----------------
*/
#pragma region debug
/* GetScanner returns the global scanner singleton. It should only be used for tests.*/
TScanner* __GetScanner();
#pragma endregion debug

#endif
