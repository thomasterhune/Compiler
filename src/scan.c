/*!
    \file scan.c
    \brief Scanner struct and 'methods' definitions.

    Scanner is responsible for tokenizing an input file. It uses the dfa defined in dfa.c to do so. It prints lines an errors to a listing file and token results to an output file.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/

/*
    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 
*/
#include "dfa.h"
#include "tokens.h"
#include "scan.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include "console.h"

/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle

struct Scanner scanner; 

void Scanner_Init() {
    scanner.line_no = 1;
    scanner.col_no = 0;
    scanner.errors = 0;
    scanner.in = NULL;
    scanner.out = NULL;
    scanner.temp = NULL;
    scanner.listing = NULL;
    scanner.buffer = NULL;
    Scanner_clearBuffer(); /* will serve to initialize the malloced buffer. */
}

void Scanner_LoadFiles(FILE * input, FILE * output, FILE * listing, FILE * temp) {
    scanner.in = input;
    scanner.out = output;
    scanner.listing = listing;
    scanner.temp = temp;
}

void Scanner_DeInit() {
    /* For now, simply call Scanner Init to reset values to zero. If additional de-initialization features are needed in the future, this is where they will go. */
    Scanner_Init();
    free(scanner.buffer);
    scanner.buffer = NULL;
}
/* DB method for testing file pos*/
FILE * Scanner_DB_GetInFile() {
    return scanner.in;
}

#pragma endregion lifecycle

/*
-------------------
Scanner buffer
-------------------
*/
#pragma region buffer

void Scanner_clearBuffer() {
    if(scanner.buffer != NULL) {
        free(scanner.buffer);
    }
    scanner.buffer = malloc(sizeof(char) * SCANNER_BUFFER_INITIAL_CAPACITY);
    scanner.capacity = SCANNER_BUFFER_INITIAL_CAPACITY;
    scanner.l_buffer = 0;
}

void Scanner_expandBuffer() {
    int new_capacity = scanner.capacity * 2;
    char * original_buffer = scanner.buffer; 
    scanner.buffer = realloc(scanner.buffer, new_capacity);
    if(scanner.buffer == NULL) {
        /* Try this if realloc fails. */
        scanner.buffer = malloc(new_capacity);
        strcpy(scanner.buffer, original_buffer);
        free(original_buffer);
    }
    scanner.capacity = new_capacity;
}

void Scanner_bufputc(char c) {
    if(scanner.l_buffer >= scanner.capacity) {
        Scanner_expandBuffer();
    }
    scanner.buffer[scanner.l_buffer] = c; 
    scanner.l_buffer += 1;
    if(scanner.l_buffer >= scanner.capacity) {
        Scanner_expandBuffer();
    }
    scanner.buffer[scanner.l_buffer] = '\0';
}

void Scanner_ReadBackToBuffer(int n_chars) {
    scanner.l_buffer = 0;
    fpos_t pos;
    fgetpos(scanner.in, &pos);
    pos -= n_chars;
    fsetpos(scanner.in, &pos);
    int i = 0;
    while(i < n_chars) {
        Scanner_bufputc(getc(scanner.in));
        i += 1;
    }
}

void Scanner_CopyBuffer(char * destination) {
    int i = 0;
    while(i < scanner.l_buffer) {
        destination[i] = scanner.buffer[i];
        i++;
    }
    destination[i] = '\0';
}

char * Scanner_GetBufferCopy() {
    char * copy = malloc(scanner.l_buffer);
    int i = 0;
    for(i = 0; i < scanner.l_buffer; i++) {
        copy[i] = scanner.buffer[i];
    }
    copy[i] = '\0';
    return copy;
}

void Scanner_PrintBuffer(FILE * destination, short print_to_console) {
    scanner.buffer[scanner.l_buffer] = '\0'; 
    fprintf(destination, "%s", scanner.buffer);
    if(print_to_console) {
        printf("%s", scanner.buffer);
    } 
}

int * Scanner_GetLBuffPointer() {
    return &scanner.l_buffer;
}

char * Scanner_GetBuffer() {
    return scanner.buffer;
}

#pragma endregion buffer

/*
-------------------
Scanning methods
-------------------
*/
#pragma region scanning

enum LHEAD_RESULT {
    LH_CLEAR, LH_NLINE, LH_EOF, LH_COMMENT
};

short Scanner_Lookahead() {

    fpos_t start_position;
    fgetpos(scanner.in, &start_position);
    

    short lookahead_result = LH_CLEAR; 

    short at_boundry = 0;
    char test;
    while(!at_boundry) {
        test = getc(scanner.in);
        if(test != ' ' && test != '\t') {
            at_boundry = 1;
            if(test == '\n') {
                lookahead_result = LH_NLINE;
            } else if(test == -1) {
                lookahead_result = LH_EOF;
            } else if(test == '-') {
                test = getc(scanner.in);
                if(test == '-') {
                    lookahead_result = LH_COMMENT;
                }
            }
        }
    }
    fsetpos(scanner.in, &start_position);
    return lookahead_result;
}

void Scanner_AdvanceLine() {
    scanner.col_no = 0;
    char test= fgetc(scanner.in);
    while(test != '\n' && test != EOF) {
        test = fgetc(scanner.in);
    }
    scanner.line_no += 1;
}

int Scanner_SkipWhitespace() {
    int n = 0;
    char c = getc(scanner.in);
    while(c == ' ' || c == '\t') {
        n += 1; 
        c = getc(scanner.in);
    }
    /* put the last character back in because it was non-ws*/
    ungetc(c, scanner.in);
    return n;
}

void Scanner_SkipAllWhitespaceForNextToken() {
    char c = getc(scanner.in);
    while(c == ' ' || c == '\t' || c == '\n') {
        c = getc(scanner.in);
    }
    if(c == '-') {
        char c2 = getc(scanner.in);
        if(c2 == '-') {
            while(c2 != '\n' && c2 != EOF) {
                c2 = getc(scanner.in);
            }
            ungetc(c2, scanner.in);
            Scanner_SkipAllWhitespaceForNextToken();
        } else {
            ungetc(c2, scanner.in);
            ungetc(c, scanner.in);
        }
    } else {
        /* put the last character back in because it was non-ws*/
        ungetc(c, scanner.in);
    }
}

/* Deprecated */
void Scanner_ScanAndPrint(FILE *input, FILE *output, FILE *listing,  FILE *temp) {
    scanner.in = input;
    scanner.listing = listing;
    scanner.out = output;
    scanner.temp = temp;

    Scanner_PrintLine();
    short look = 0;
    int charsRead = 0;
    int token;
    while(look != LH_EOF) {
        look = Scanner_Lookahead();
        switch(look) {
            case LH_NLINE:
                Scanner_AdvanceLine();
                Scanner_PrintLine();
                break;
            case LH_COMMENT:
                Scanner_AdvanceLine();
                Scanner_PrintLine();
                break;
            case LH_EOF:
                break;
            default: /* Clear to scan */
                scanner.col_no += Scanner_SkipWhitespace();
                token = GetNextToken(scanner.in, &charsRead);
                Scanner_PrintTokenFront(token);
                Scanner_ReadBackToBuffer(charsRead);
                Scanner_PrintBufferToOutputFile();
                if(token == LEX_ERROR) {
                    Scanner_PrintErrorListing();
                    scanner.errors += 1;
                }
                scanner.col_no += charsRead;
                
                break;
        }
    }
    Scanner_PrintTokenFront(SCANEOF);
    Scanner_BackprintIdentifier(0);
    Scanner_PrintErrorSummary(1);

}

int Scanner_NextToken() {
    fpos_t pos;
    fgetpos(scanner.in, &pos);
    Scanner_SkipAllWhitespaceForNextToken();
    int charsRead = 0;
    int token = GetNextToken(scanner.in, &charsRead);
    Scanner_ReadBackToBuffer(charsRead);
    int v = fsetpos(scanner.in, &pos);
    return token;
}

void Scanner_SkipLexError() {
    /* Skip an error, but print it to the listing file */
    scanner.errors += 1;
    scanner.col_no += 1;
    char problem = scanner.buffer[0]; 
    fprintf(scanner.listing, "      \t");
    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        printf("      \t");
    }

    /* Add spaces to align the caret with the error. */
    int i = 0;
    while(i < scanner.col_no) {
        fputc(' ', scanner.listing);
        if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
            putchar(' ');
        }
        i++;
    }

    /* Print the indicator caret. */
    fputc('^', scanner.listing);
    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        putchar('^');
    }

    /* Print the error message. */    
    fprintf(scanner.listing, "Error. '%c' not recognized at line %d col %d.\n", problem, scanner.line_no, scanner.col_no);

    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        printf("Error. '%c' not recognized at line %d col %d.\n", problem, scanner.line_no, scanner.col_no);
    }

}

short Scanner_Match(int target_token) {
    /* int v = fprintf(scanner.out, "\nExpected Token: %15s ", Token_GetName(target_token)); */
    if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
        /*printf("\nExpected Token: %s ", Token_GetName(target_token));*/
    }
    int token;
    int charsRead = 0;
    short look = 1;
    short result;
    short hitEOF = 0;
    while(look != LH_CLEAR && hitEOF == 0) {
        /* while look is not 'clear to scan' */
        look = Scanner_Lookahead();
        switch(look) {
            case LH_NLINE:
                Scanner_AdvanceLine();
                Scanner_PrintLine();
                break;
            case LH_COMMENT:
                Scanner_AdvanceLine();
                Scanner_PrintLine();
                break;
            case LH_EOF:
                hitEOF = 1;
                break;
            default: /* clear to scan */
                break;
        }
    }
    if(hitEOF == 1) {
        if(target_token == SCANEOF) {
            result = 1;
        } else {
            result = 0;
        }
        /*fprintf(scanner.out, "    Actual Token: %15s", Token_GetName(target_token));*/
        if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
            printf(" Actual Token: %s ", Token_GetName(target_token));
        }
    } else {
        token = GetNextToken(scanner.in, &charsRead);
        Scanner_ReadBackToBuffer(charsRead);
        scanner.col_no += charsRead;
        if(token == target_token) {
            result = 1;
            if(token == LEX_ERROR) {
                Scanner_SkipLexError();
            }
        } else {
            result = 0;
        }
        /* fprintf(scanner.out, "    Actual Token: %15s :: ", Token_GetName(token)); */
        if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
            printf(" Actual Token: %s :: ", Token_GetName(token));
        }
        /* Scanner_PrintBuffer(scanner.out, SCANNER_PRINTS_TOKENS_TO_CONSOLE); */
        if(result) {
            Parser_pushToBuffer(scanner.buffer, scanner.l_buffer);
        }
    }


    return !result;
}

#pragma endregion scanning 

/*
-------------------
Printing methods
-------------------
*/
#pragma region printing

void Scanner_PrintLine() {
    /* Prepare to print the line by getting the file position and printing the line number. */
    fpos_t start;
    fgetpos(scanner.in, &start);
    fprintf(scanner.listing, " %3d :\t", scanner.line_no);
    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        printf("\n\n>> LINE");
        printf("\n %3d :\t", scanner.line_no);
    }

    /* Print the line. */
    char read = getc(scanner.in);
    while(read != '\n' && read != EOF) {
        fputc(read, scanner.listing);
        if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
            putchar(read);
        }
        read = getc(scanner.in);
    }
    /* print the newline, but not the EOF character. */
    if(read == '\n') {
        fputc(read, scanner.listing);
        if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
            putchar(read);
        }
    }

    /* reset the file position. */
    fsetpos(scanner.in, &start);
}

void Scanner_BackprintIdentifier(int nchars) {
    /* Move the file position back nchars. */
    fpos_t pos;
    fgetpos(scanner.in, &pos);
    pos -= nchars;
    fsetpos(scanner.in, &pos);
    int i = 0;
    char c;
    /* Print those characters, moving the filepos back where it started. */
    while(i < nchars) {
        c = fgetc(scanner.in);
        if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
            putchar(c);
        }
        /* fputc(c, scanner.out); */
        i++;
    }
}

void Scanner_PrintBufferToOutputFile() {
    int i = 0;
    char c;
    while(i < scanner.l_buffer) {
        c = scanner.buffer[i];
        if(c == '\n') {
            printf("<< newline at %d of %d>>>", i , scanner.l_buffer);
        }
        /* fputc(c, scanner.out); */
        if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
            putchar(c);
        }
        i++;
    }
}

void Scanner_PrintTokenFront(int token) {
    /* fprintf(scanner.out, "\ntoken number:  %d\ttoken type:  %12s \t actual token:  ", token, Token_GetName(token)); */
    if(SCANNER_PRINTS_TOKENS_TO_CONSOLE) {
        printf("\ntoken number: %4d  token type:  %12s   actual token:  ", token, Token_GetName(token));
    }
}

void Scanner_PrintErrorListing() {
    /* Move the file pointer back by 1 in the input file to get the error-causing character. */
    fpos_t pos;
    char problem;
    fgetpos(scanner.in, &pos);
    pos -= 1;
    fsetpos(scanner.in, &pos);
    problem = getc(scanner.in);

    /* Indent over past line number area. */
    fprintf(scanner.listing, "      \t");
    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        printf("      \t");
    }

    /* Add spaces to align the caret with the error. */
    int i = 0;
    while(i < scanner.col_no) {
        fputc(' ', scanner.listing);
        if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
            putchar(' ');
        }
        i++;
    }

    /* Print the indicator caret. */
    fputc('^', scanner.listing);
    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        putchar('^');
    }

    /* Print the error message. */    
    fprintf(scanner.listing, "Error. '%c' not recognized at line %d col %d.\n", problem, scanner.line_no, scanner.col_no);

    if(SCANNER_PRINTS_LINES_TO_CONSOLE) {
        printf("Error. '%c' not recognized at line %d col %d.\n", problem, scanner.line_no, scanner.col_no);
    }
}

void Scanner_PrintErrorSummary() {
    if(scanner.errors == 1) {
        fprintf(scanner.listing, "\n\n%d Lexical Error.", scanner.errors);
        printf("\n\n%d Lexical Error.", scanner.errors);
    } else {
        fprintf(scanner.listing, "\n\n%d Lexical Errors.", scanner.errors);
        printf("\n\n%d Lexical Errors.", scanner.errors);
    }
}

void Scanner_PrintParseErrorMessage() {
    fprintf(scanner.listing, "\n      PARSE ERROR! Line %d! See output file for details.\n", scanner.line_no + 1);
    printf("\n");
    CONSOLE_COLOR(FG_BRT_RED, BG_BLUE);
    printf(" PARSE ERROR ");
    CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
    printf(" Line: %d", scanner.line_no+1);
    CONSOLE_COLOR_DEFAULT();
}

int Scanner_GetLexErrCount(){
    return scanner.errors;
}

#pragma endregion printing