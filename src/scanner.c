#include "scanner.h"
#include "scanner_util.h"

#include <stdlib.h>

/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle

void Scanner_Init(FILE * in, FILE * out, FILE * listing, FILE * tmp) {
    scanner.buf_pos = 0;
    scanner.line_count = 0;
    scanner.in = in;
    scanner.out = out;
    scanner.listing = listing;
    scanner.temp = tmp;
    scanner.buffer = malloc(sizeof(char) * TSCANNER_INIT_BUFF_SIZE);
    scanner.current_buff_size = TSCANNER_INIT_BUFF_SIZE;
}

void Scanner_DeInit() {
    if(scanner.buffer != NULL) {
        free(scanner.buffer);
        scanner.buffer = NULL;
    }
    scanner.in = NULL;
    scanner.out = NULL;
    scanner.listing = NULL;
    scanner.temp = NULL;
}

#pragma endregion lifecycle

/*
-------------------
Scanner buffer
-------------------
*/

#pragma region buffer

void Scanner_clearBuffer() {
    scanner.buffer = refreshBuffer(scanner.buffer, TSCANNER_INIT_BUFF_SIZE);
    scanner.current_buff_size = TSCANNER_INIT_BUFF_SIZE;
    scanner.buf_pos = 0;
}

void Scanner_expandBuffer() {
    scanner.current_buff_size *= 2;
    scanner.buffer = resizeBuffer(scanner.buffer, scanner.current_buff_size);
}

int Scanner_populateBuffer() {
    /* Each time we read the buffer, increment the line count. */
    scanner.line_count += 1;
    /* If the buffer was previously expander, we can shrink it again for the next read. */
    if(scanner.current_buff_size > TSCANNER_INIT_BUFF_SIZE) {
        Scanner_clearBuffer();
    }
    char check = '\0';
    int read_index = 0;
    while( check != '\n' && check != EOF) {
        /* Expand the buffer if it isn't big enough. */
        if(read_index >= scanner.current_buff_size - 1) {
            Scanner_expandBuffer();
        }
        check = getc(scanner.in);
        scanner.buffer[read_index] = check;
        read_index++;
    }
    return read_index; /* The number of characters read. */
}

#pragma endregion buffer

/*
----------------
Scanner logic
----------------
*/

#pragma region logic

/*! Possible lookahead values.

    SCAN_LHEAD_WHITESPACE   The current char is a whitespace
    SCAN_LHEAD_EOF     The current char is EOF
    SCAN_LHEAD_NLINE    The current char is newline
    SCAN_LHEAD_COMMENT  The current AND next char are '-'.
    SCAN_LHEAD_SYNTAX   The current char is none of the above and may be valid syntax.
*/
enum SCAN_LHEAD_VAL {
    SCAN_LHEAD_WHITESPACE=0, SCAN_LHEAD_EOF, SCAN_LHEAD_NLINE, SCAN_LHEAD_COMMENT, SCAN_LHEAD_SYNTAX
};

void Scanner_Scan() {
    short at_eof = 0;
    int chars_read = 0;
    short lookahead = SCAN_LHEAD_SYNTAX;
    chars_read = Scanner_populateBuffer();

    while(at_eof == 0) {
        lookahead = Scanner_lookAhead();
        switch(lookahead) {
            case SCAN_LHEAD_COMMENT:
            /* If '--' are upcoming, we can skip tokenizing the rest of the line*/
                break;
            case SCAN_LHEAD_EOF:
            /* If eof is upcoming, we are done.*/
                at_eof = 1;
                break;
            case SCAN_LHEAD_NLINE:
            /* time to read another line. */
                chars_read = Scanner_populateBuffer();
                break;
            case SCAN_LHEAD_WHITESPACE:
            /* whitespace to skip*/
                /* Scanner_SkipWhitespace()? */
                break;
            default:
                /* If it's none of the above it's SCAN_LHEAD_SYNTAX*/
                /* Scanner_ExtractWord()? */
                break;
                
        }
    }
}

short Scanner_lookAhead() {
    char c = scanner.buffer[scanner.buf_pos];
    short lookahead_return = SCAN_LHEAD_SYNTAX;
    if(c == '\n') {
        lookahead_return = SCAN_LHEAD_NLINE;
    } else if(c == EOF) {
        lookahead_return = SCAN_LHEAD_EOF;
    } else if(c == ' ' || c == '\t') {
        lookahead_return = SCAN_LHEAD_WHITESPACE;
    } else if(c == '-') {
        c = scanner.buffer[scanner.buf_pos+1];
        /* It's safe to look ahead again; the buffer will always end with a \n or a new line. */
        if(c == '-') {
            lookahead_return = SCAN_LHEAD_COMMENT;
        }
    }
    return lookahead_return;
}


#pragma endregion logic

/*
---------------
Scanner actions
---------------
*/
#pragma region action

void Scanner_Skipwhitespace()
{
    /* can use getchar to skip the whitespace im not sure what we are passing in here and how to acess it like is it a spefic postion(like a single char
        or many 
    
    */

   /* Shouldn't have to pass anything in. That lookahead enum is only used by the switch statement in Scanner_Scan(). I created a skip whitespace in scanner_util.c - this can probably just call that with members of scanner. */
    
    
    
}



void Scanner_ExtractWord()
{
/*same for here use getchar and extract entire word that is valid synatx just go until you reahed whitespace or some other delimter 


*/


}




#pragma endregion action

/*
----------------
Scanner debug
----------------
*/
#pragma region debug

/* GetScanner returns the global scanner singleton. It should only be used for tests.*/
TScanner* __GetScanner() {
    return &scanner;
}

#pragma endregion debug



