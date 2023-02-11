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
typedef struct {
    /*A character array for the input buffer*/
    char * buffer;
    /*! The current buffer size, in bytes. */
    int current_buff_size; 
    /*! The current position being read in the buffer and a running counter of the column being read.*/
    int buf_pos;
    /*! A running counter of the line being read. */
    int line_count;
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

} TScanner;

/*! The singleton instance of the scanner. */
TScanner scanner;

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

/*!
    Reads a file until a newline character or EOF character is found. Puts all found characters in its buffer. Expands the buffer if necessary to fit a line.

    It also increments scanner's line counter and resets buf_pos (which doubles as a column counter) to 0.
    
    \returns The number of characters read into the buffer.
    \author klm127
    \date 2/10/2023
*/
int Scanner_populateBuffer();

#pragma endregion buffer

/*!
Scanner_addChar add a char to the buffer

*/
void Scanner_addChar();


#pragma region logic


/*!
    Executes calls to other Scanner methods to perform the scanning logic. Uses Scanner_Scan to look ahead and determine the correct function to call with a switch statement.

*/
void Scanner_Scan();
/*!
    Peeks ahead in the scanner. Starts by looking at current position and scans forward if a '-' is found so it can identify a comment. It does not move the buf pos.
    \returns A SCAN_LHEAD_VAL corresponding to what it found. 0 = whitespace, 1 = eof, 2 = newline, 3 = comment, 4 = syntax
    \author klm127
    \date 2/10/2023
*/
short Scanner_lookAhead();


/*!
    will skip over whitespace and not put it in the buffer
*/
void Scanner_Skipwhitespace();


/*!
    will extract a word from the lookahead because it will be a synatx error
*/
void Scanner_ExtractWord();

#pragma endregion logic

#pragma region debug
/* GetScanner returns the global scanner singleton. It should only be used for tests.*/
TScanner* __GetScanner();
#pragma endregion debug

#endif
