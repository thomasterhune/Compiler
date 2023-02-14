#include "scanner.h"
#include "scanner_util.h"

#include <stdlib.h>
#include <string.h>


/*!
    \file scanner.c
    \brief Scanner implementation

    As with our other files, the majority of commenting is done above function declaration in the header file (scanner.h). This is to enable intellisense in modern IDEs. Doing comments this way allows a user of, say, VSCode, to hover over a function and get a popup box describing the function's parameters, return values, preconditions, and so forth. 

*/

/*
-----------------------------------
Consts related to language features
-----------------------------------
*/

char * operators = "();,:=+-*/!<>";
char * alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char * numbers = "0123456789";

/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle

void Scanner_Init(FILE * in, FILE * out, FILE * listing, FILE * tmp) {
    scanner.on_last_line = 0;
    scanner.n_errors = 0;
    scanner.line_count = 0;
    scanner.buf_pos = 0;
    scanner.in = in;
    scanner.out = out;
    scanner.temp = tmp;
    scanner.listing = listing;
    scanner.current_buff_size = TSCANNER_INIT_BUFF_SIZE;
    scanner.buffer = malloc(sizeof(char) * TSCANNER_INIT_BUFF_SIZE);

    /* It's a little tricky to sneak the EOF characters in the boundries array. The boundries array determines which characters can terminate an int literal, keyword, and identifier.  */
    scanner.boundaries = malloc( (strlen(operators)+5)*sizeof(char));
    strcpy(scanner.boundaries, operators);
    strcat(scanner.boundaries, "\n\t ");
    int i = strlen(scanner.boundaries);
    scanner.boundaries[i] = EOF;
    scanner.boundaries[i+1] = '\0';

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
    free(scanner.boundaries);
    scanner.boundaries = NULL;
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
    /* If the buffer was previously expanded, we can shrink it again for the next read. */
    if(scanner.current_buff_size > TSCANNER_INIT_BUFF_SIZE) {
        Scanner_clearBuffer();
    }
    char check = '\0';
    int read_index = 0;
    while( check != '\n' && check != EOF) {
        /* Expand the buffer if it isn't big enough. */
        if(read_index >= scanner.current_buff_size - 2) {
            Scanner_expandBuffer();
        }
        check = getc(scanner.in);
        /* The newline and EOF characters WILL be read into the buffer. */
        scanner.buffer[read_index] = check;
        read_index++;
    }
    scanner.buffer[read_index] = '\0';
    if(check == EOF) {
        scanner.on_last_line = 1;
    }
    scanner.buf_pos = 0;
    Scanner_printLine();
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
    SCAN_LHEAD_EOF          The current char is EOF
    SCAN_LHEAD_NLINE        The current char is newline
    SCAN_LHEAD_COMMENT      The current AND next char are '-'.
    SCAN_LHEAD_OPERATOR     The current char is an operator
    SCAN_LHEAD_NUMBER       The current char is a number
    SCAN_LHEAD_WORD         The current char is none of the above and may be valid syntax.
    SCAN_LHEAD_ERROR        The current char is not in the alphabet, operators, or numbers.
*/
enum SCAN_LHEAD_VAL {
    SCAN_LHEAD_WHITESPACE=0, SCAN_LHEAD_EOF, SCAN_LHEAD_NLINE, SCAN_LHEAD_COMMENT, SCAN_LHEAD_OPERATOR, SCAN_LHEAD_NUMBER, SCAN_LHEAD_WORD, SCAN_LHEAD_ERROR
};

void Scanner_Scan(FILE * in, FILE * out, FILE * listing, FILE * temp) {
    scanner.in = in;
    scanner.out = out;
    scanner.listing = listing;
    scanner.temp = temp;
    int chars_read;
    short lookahead = 0;

    Scanner_populateBuffer();
    while(lookahead != SCAN_LHEAD_EOF) {
        lookahead = Scanner_lookAhead();
        Scanner_takeAction(lookahead);
    }
    Scanner_PrintErrorCount();
}

short Scanner_lookAhead() {
    char c = scanner.buffer[scanner.buf_pos];
    short lookahead_return = SCAN_LHEAD_WORD;
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
        } else {
            lookahead_return = SCAN_LHEAD_OPERATOR;
        }
    } else if(c >= '0' && c <='9') {
        lookahead_return = SCAN_LHEAD_NUMBER;
    }else {
        /* See if the character is an operator by checking it against the list. */
        short isop = charIn(c, operators, strlen(operators));
        if(isop) {
            lookahead_return = SCAN_LHEAD_OPERATOR;
        } else {
            /* See if the character is an alphanumeric by checking it against the list. */
            short ischar = charIn(c, alphabet, strlen(alphabet));
            if(ischar) {
                lookahead_return = SCAN_LHEAD_WORD;
            } else {
                lookahead_return = SCAN_LHEAD_ERROR;
            }
        }
    }
    return lookahead_return;
}

void Scanner_takeAction(short lookAheadResult) {
    char * tokenFound;
    char * tokTextOp;
    int tok;
    struct TokenCatch* tok_catch = NULL;
    int token_start_at = scanner.buf_pos;
    int chars_to_print;
    switch(lookAheadResult) {
        case SCAN_LHEAD_COMMENT:
            Scanner_populateBuffer(); /* move along to the next line . */
            break;
        case SCAN_LHEAD_EOF:
            tok_catch = Token_Catch(SCANEOF, "<file end>", scanner.line_count, token_start_at);
            Scanner_printToken(tok_catch);
            break;
        case SCAN_LHEAD_NLINE:
            Scanner_populateBuffer(); /* move along to the next line. */
            break;
        case SCAN_LHEAD_WHITESPACE:
            Scanner_Skipwhitespace();
            break;
        case SCAN_LHEAD_OPERATOR:
            tok = Scanner_ExtractOperator();
            tok_catch = Token_CatchOp(tok, scanner.line_count, token_start_at);
            Scanner_printToken(tok_catch);
            break;
        case SCAN_LHEAD_NUMBER:
            tokenFound = Scanner_ExtractInteger();

            tok_catch = Token_Catch(INTLITERAL, tokenFound, scanner.line_count, token_start_at);
            Scanner_printToken(tok_catch);
            break;
        case SCAN_LHEAD_ERROR:
            tok_catch = Token_CatchError(scanner.buffer[scanner.buf_pos], scanner.line_count, token_start_at);
            Scanner_printError(tok_catch);
            scanner.buf_pos += 1;
            scanner.n_errors += 1;
            
            break;
        default:
            /* If it's none of the above it's SCAN_LHEAD_WORD*/
            tokenFound = Scanner_ExtractWord();
            tok = Token_RecognizeKeyword(tokenFound, strlen(tokenFound));
            tok_catch = Token_Catch(tok, tokenFound, scanner.line_count, token_start_at);
            Scanner_printToken(tok_catch);
            break;
    }
}
#pragma endregion logic

/*
---------------
Scanner actions
---------------
*/
#pragma region actions

void Scanner_Skipwhitespace()
{
    skipWhitespace(scanner.buffer, &scanner.buf_pos);   

}

char* Scanner_ExtractWord()
{
    return extractWord(scanner.buffer, &scanner.buf_pos, scanner.boundaries, strlen(scanner.boundaries));

}

char* Scanner_ExtractInteger() 
{
    return extractInt(scanner.buffer, &scanner.buf_pos);
}

int Scanner_ExtractOperator()
{
    return extractOperator(scanner.buffer, &scanner.buf_pos);
}

int extractOperator(char *buffer, int *index) {
    int retval = ERROR;
    switch (buffer[*index]){
        case '(':
            retval = LPAREN;
            break;
        case ')':
            retval = RPAREN;
            break;
        case ';':
            retval = SEMICOLON;
            break;
        case ',':
            retval = COMMA;
            break;
        case ':':
            if(buffer[(*index) + 1] == '='){
                retval = ASSIGNOP;
                *index = *index + 1;
            }else{
                retval = ERROR;
            }
            break;
        case '+':
            retval = PLUSOP;
            break;
        case '-':
            retval = MINUSOP;
            break;
        case '*':
            retval = MULTOP;
            break;
        case '/':
            retval = DIVOP ;
            break;
        case '!':
            retval = NOTOP;
            break;
        case '<':
            if(buffer[(*index) + 1] == '='){
                retval = LESSEQUALOP;
                *index = *index + 1;
            }
            else if (buffer[(*index) + 1] == '>'){
                retval = NOTEQUALOP;
                *index = *index + 1;
            }else{
                retval = LESSOP;
            }
            break;
        case '>':
            if(buffer[(*index) + 1] == '='){
                retval = GREATEREQUALOP;
                *index = *index + 1;
            }else{
                retval = GREATEROP;
            }
            break;
        case '=':
            retval = EQUALOP;
            break;
        case EOF:
            retval = SCANEOF;
            break;
        default:
            break;
    }
    *index = *index + 1;
    return retval;
}

#pragma endregion actions
/*
----------------
Scanner printing
----------------
*/
#pragma region printing

void Scanner_printLine() {
    fprintf(scanner.listing, "% 3d :\t", scanner.line_count);
    if(SCANNER_PRINTS_TO_CONSOLE) {
        printf("\n%d:\t", scanner.line_count);
    }
    int index = 0;
    char c = scanner.buffer[index];
    while(c != '\n' && c != EOF) {
        fputc(scanner.buffer[index], scanner.listing);
        if(SCANNER_PRINTS_TO_CONSOLE) {
            printf("%c", scanner.buffer[index]);
        }
        index++;
        c = scanner.buffer[index];
        /* printc*/
    }
    fprintf(scanner.listing, "\n");
}

void Scanner_printToken(struct TokenCatch * token){
    if(SCANNER_PRINTS_TO_CONSOLE){
        printf("\ntoken number: %d\t token type: %s\t actual token: %s", token->token, Token_GetName(token->token), token->raw);
    }
    fprintf(scanner.out, "\ntoken number: %d\t token type: %s\t actual token: %s", token->token, Token_GetName(token->token), token->raw);
    
}

void Scanner_printError(struct TokenCatch * token){
    fprintf(scanner.listing, "% 3d x\t", scanner.line_count);
    int i = 0;
    for (i = 0; i < token->col_no; i++){
        fprintf(scanner.listing, " ");
        if(SCANNER_PRINTS_TO_CONSOLE) {
            printf(" ");
        }
    }
    if(SCANNER_PRINTS_TO_CONSOLE){
        printf("\n^ERROR. %s not recognized at Line %d Column %d.\n", token->raw, token->line_no, token->col_no);
    }
    fprintf(scanner.listing, "^ERROR. %s not recognized at Line %d Column %d.\n", token->raw, token->line_no, token->col_no);
}

void Scanner_PrintErrorCount() {
    fprintf(scanner.listing, "\n\n%d syntactic errors. \n", scanner.n_errors);
    if(SCANNER_PRINTS_TO_CONSOLE) {
        printf("\n\n%d syntactic errors. \n", scanner.n_errors);
    }
}
#pragma endregion printing
/*
----------------
Scanner debug
----------------
*/
#pragma region debug

/* __GetScanner returns the global scanner singleton. It should only be used for tests. */
TScanner* __GetScanner() {
    return &scanner;
}

#pragma endregion debug



