#ifndef dfa_h
#define dfa_h
/*!
    \file dfa.h
    \brief The DFA and related logic declarations.

    The DFA is a 3 dimensional array that maps a given state and character input to a result consisting of the next state, token, and whether reading should continue.

    The DFA was created in Excel, and the excel file is available in docs/fullDFA.xlsx.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/

#include <stdio.h>

/*!
   Translates a given character into a column index of the state-transition table.
*/
short GetDFAColumn(char c);

/*!
    Gets the next token from the file. Skips leading whitespace. Sets charsRead to the number of characters read, not including whitespace skipped. 
    \param file A file to read for tokens.
    \param charsRead A pointer to an int. The value at charsRead will be overwritten with the number of chars read. 
    \returns An int representing a token. See tokens.c. 
*/
int GetNextToken(FILE * file, int * charsRead);


/*!
    Gets the next token from a buffer. Skips leading whitespace. Sets charsRead to the number of characters read, not including whitespace skipped. 

    \note This function primarily exists to test the DFA itself against buffers rather than passing in files. 
    \param buffer a character buffer
    \param bufIndex 
    
*/
int GetNextTokenInBuffer(char * buffer, int * bufIndex, int * charsRead);


/*! A debug function for printing a cell in the DFA. */
void printCell(int row, int col);

/*! A debug function for printing a state (row name) and column (char name)*/
void printStateAndChar(int row, int col);

#endif