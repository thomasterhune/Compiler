#ifndef scan_h
#define scan_h

#include <stdio.h>
/*!
    \file scan.h
    \brief Scanner struct and 'methods' declarations.

    Scanner is responsible for tokenizing an input file. It uses the dfa defined in dfa.c to do so. It prints lines an errors to a listing file and token results to an output file.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/

/*
-------------------
Flags
-------------------
*/

#ifndef SCANNER_PRINTS_LINES_TO_CONSOLE
#define SCANNER_PRINTS_LINES_TO_CONSOLE 1
#endif

#ifndef SCANNER_PRINTS_TOKENS_TO_CONSOLE
#define SCANNER_PRINTS_TOKENS_TO_CONSOLE 1
#endif 

#ifndef SCANNER_BUFFER_INITIAL_CAPACITY
#define SCANNER_BUFFER_INITIAL_CAPACITY 100
#endif 

/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle
/*!
    Scanner struct holds references to the files being read and keeps track of the line and column position. It is a singleton. 
*/
struct Scanner {
    /*! The line number being scanned. */
    int line_no;
    /*! The column number. */
    int col_no;
    /*! The error count. */
    int errors;
    /* A buffer, primarily for capturing identifiers. */
    char * buffer;
    /* The current capcity of the buffer */
    int capacity;
    /* The length of relevant characters in the buffer, also the write index.  */
    int l_buffer; 
    /* File pointers. */
    FILE * in;
    FILE * out;
    FILE * temp;
    FILE * listing;
};

/*! Initializes scanner values to zero. */
void Scanner_Init();

/*! Loads input, output, listing, and temp files for scanner referencing.

    \param input The input file which will be scanned. 
    \param output The output file.
    \param listing The listing file.
    \param temp The temp file. 
*/
void Scanner_LoadFiles(FILE * input, FILE * output, FILE * listing, FILE * temp);

/*! De-initializes scanner values, setting file pointers to NULL ( but not closing files. ) */
void Scanner_DeInit();

#pragma endregion lifecycle

/*
-------------------
Scanner buffer
-------------------
*/
#pragma region buffer

/*! 
    If scanner.buffer has been allocated, it is freed. Scanner.buffer is given memory on the heap equal to the const SCANNER_BUFFER_INITIAL_CAPACITY. 

    Also resets scanner.l_buffer to 0 and scanner.capacity. 

*/
void Scanner_clearBuffer();

/*!
    Doubles the size of the buffer, preserving data in the buffer (But possibly moving it to a different location). Scanner.buffer will point to a buffer twice as large and scanner.capacity will be doubled. If necessary, an old buffer may have been freed. Do not use scanner's buffer directly as it may be freed; copy the string to a new buffer when extraction is necessary. 
*/
void Scanner_expandBuffer();

/*!
    Puts a character in the buffer. Increments scanner.l_buffer. Calls expandBuffer() if necessary. 
*/
void Scanner_bufputc(char c);

/*!
    Reads back a number of characters from scanner.in into the buffer. The file pointer will be in the same position after execution. 
*/
void Scanner_ReadBackToBuffer(int n_chars);


/*! 
    Copies the contents of scanners buffer to another char array destination. Appends a null terminator '\0' to the end of that buffer as well.
    \param destination The string to copy to. 
*/
void Scanner_CopyBuffer(char * destination);


#pragma endregion buffer

/*
-------------------
Scanning methods
-------------------
*/
#pragma region scanning
/*!
    Scans a file for tokens and prints detailed information to the listing and output files.

    \warning This function was used for validating scanner functionality, it is not used when the program is parsing. 
    \deprecated

    \param input An input file pointer, already opened for reading.
    \param listing An listing file pointer, already opened for writing.
    \param output An output file pointer, already opened for writing.
    \param temp An temp file pointer, already opened for writing.
*/
void Scanner_ScanAndPrint(FILE *input, FILE *listing, FILE *output, FILE *temp);

/*!
    Looks ahead to determine if there are any more tokens on the line, or if there is a comment at the end of the line. Resets the fileposition after looking ahead.

    This is an internal method of scanner used to determine how it should scan. It should not be called by external modules. 
    \private

    \returns 0 = Clear to Scan,
            1 = Newline next,
            2 = EOF next,
            3 = Comment next,
*/
short Scanner_Lookahead();

/*!
    Advances the file pointer until the start of the next line. Increments the line-number counter in scanner. 
*/
void Scanner_AdvanceLine();

/*!
    Advances the file pointer until a nonwhitespace character (not space or tab) and returns the number of characters skipped.

    While the DFA can skip whitespace independently, using this method allows tracking the number of characters that were skipped to maintain an accurate column number.

    \private

    \returns The number of whitespace characters skipped. 
*/
int Scanner_SkipWhitespace();

/*!
    Advances the file pointer until a nonwhitespace character (not space or tab or newline) and returns the number of characters skipped.

    It does NOT advance the column number.

    This is used for Scanner_NextToken, so that the next token can be checked without advancing the column count or printing the lines (which will only occur on match.)
*/
void Scanner_SkipAllWhitespaceForNextToken();

/*!
    Scans the next token in scanner.in. Moves the file pointer back where it started.

    \returns The next token in the input file. 

*/
int Scanner_NextToken();

/*!
    Consumes the next token in scanner.in.

    \todo Checks for newlines and prints to the listing file the line if one is found. 
    \todo Advances column position.

    \returns 1 if token is matched correctly. 0 if the tokens do not match. 

*/
short Scanner_Match(int target_token);

#pragma endregion scanning

/*
-------------------
Printing methods
-------------------
*/
#pragma region printing

/*!
    Prints a line with a line number to the listing file. Will print newlines but will not print EOFs. Resets the file pointer to its original position after printing the line.
*/
void Scanner_PrintLine();

/*!
    Moves the file pointer of scanner.in back nchars and prints that many chars to the output file and possibly the console. Used for printing the actual text of a token.
    \param nchars The number of characters to backprint.
*/
void Scanner_BackprintIdentifier(int nchars);

/*!
    Prints the buffer to the output file, expecting a null-terminated string. Also prints it to the console if console printing is enabled. 
*/
void Scanner_PrintBufferToOutputFile();

/*!
    Prints the token output to the output file and possibly the console. Example: \ntoken number:  0    token type:  BEGIN     actual token: After calling, Scanner_BackprintIdentifier should be called to print the actual token. 
*/
void Scanner_PrintTokenFront();

/*!
    Prints an error message to the listing file and possibly the console. Example: \nError. & not recognized.  
*/
void Scanner_PrintErrorListing();

/*!
    Prints the total error count to the listing file and possibly the console. 
*/
void Scanner_PrintErrorSummary();

#pragma endregion printing


#endif