/*!
    \file parse.c
    \brief Parser struct, 'methods' definitions including Parse functions.

    Parse is responsible for validating the syntax of an input file. It reads tokens provided by the scanner and validates that their sequence conforms with the rules of the language. 

    Parse_SystemGoal is the entry point, which should be called only after input and output files are loaded into the scanner and parser. It calls for function corresponding to each unique LHS of a production rule. 

    If a lexical error is encountered (invalid character), the character is skipped and an error is printed to the listing file. Parsing will continue with the next available character.

    If a syntax error is encountered within a statement, tokens will be skipped until a semicolon or other end-of-statement symbol is found and information about that error will be printed as a trace in the console and the files. 

    \authors Karl Miller, Tom Terhune, Anthony Stepich
    \date March 2023
*/
/*
    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 
*/
#include "parse.h"
#include <stdio.h>
#include "tokens.h"
#include <string.h>
#include "scan.h"
#include "console.h"
#include "stdarg.h"

T_Parser parser;

/*
--------------------------------
Lifecycle methods for the parser
---------------------------------
*/
#pragma region lifecycle

void Parser_Load(FILE *out, FILE *list) {
    parser.out = out;
    parser.list = list;
}

void Parser_Init() {
    parser.errorCount = 0;
    parser.trace = 0;
    Parser_clearBuffer(); /* sets buffer, capacity, l_buffer */
}

void Parser_DeInit() {
    parser.out = NULL;
    parser.list = NULL;
    if(parser.buffer != NULL) {
        free(parser.buffer);
    }
    parser.buffer = NULL;
}

#pragma endregion lifecycle

/*
-------------------
Parser buffer
-------------------
*/
#pragma region buffer


void Parser_expandBuffer() {
    int new_capacity = parser.capacity * 2;
    char * original_buffer = parser.buffer; 
    parser.buffer = realloc(parser.buffer, new_capacity);
    if(parser.buffer == NULL) {
        /* Try this if realloc fails. */
        original_buffer[parser.l_buffer] = '\0';
        parser.buffer = malloc(new_capacity);
        strcpy(parser.buffer, original_buffer);
        free(original_buffer);
    }
    parser.capacity = new_capacity;
}

void Parser_clearBuffer() {
    if(parser.buffer != NULL) {
        free(parser.buffer);
    }
    parser.buffer = malloc(sizeof(char) * PARSER_BUFFER_INITIAL_CAPACITY);
    parser.capacity = PARSER_BUFFER_INITIAL_CAPACITY;
    parser.l_buffer = 0;
}

void Parser_pushToBuffer(char * word, int l_word) {
    int i = parser.l_buffer;
    int end = i + l_word;
    while(end + 1 > parser.capacity) {
        Parser_expandBuffer();
    }
    int j = 0;
    while(i < end && j < l_word) {
        parser.buffer[i] = word[j];
        j++; i++;
    }
    parser.l_buffer = end;
    parser.buffer[end] = '\0';
}


void Parser_printBufferStatementToOutAndClear() {
    fprintf(parser.out, "\n\nStatement: %s\n", parser.buffer);
    parser.l_buffer = 0;
}

#pragma endregion buffer

/*
-------------------
Parse errors
-------------------
*/
#pragma region parse_errors

void ParseError_MatchFailed(int expected_token) {
    parser.errorCount += 1;
    Scanner_PrintParseErrorMessage();
    CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
    int i = 0;
    fputc('\n', parser.out);
    fputc('\n', stdout);
    while(i < parser.trace) { /* Print at the same indentation level of a function fail*/
        fputc(' ', parser.out);
        fputc(' ', stdout);
        i++;
    }
    fprintf(parser.out, "\nFailed to match expected token: %s", Token_GetName(expected_token));
    printf("Failed to match expected token: %s", Token_GetName(expected_token));
    CONSOLE_COLOR_DEFAULT();
}


void ParseError_NextTokenFailed(int actual_token, int n_expected, ...) {
    parser.errorCount += 1;
    Scanner_PrintParseErrorMessage();
    CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
    int i = 0;
    fputc('\n', parser.out);
    fputc('\n', stdout);
    while(i < parser.trace) {
        fputc(' ', parser.out);
        fputc(' ', stdout);
        i++;
    }
    fprintf(parser.out, "Next token %s was not one of the expected tokens: ", Token_GetName(actual_token));
    printf("Next token %s was not one of the expected tokens: ", Token_GetName(actual_token));

    /* Print the variadic list of expected next token values. */
    va_list ptr;
    va_start(ptr, n_expected);
    int t;
    i = 0;
    while(i < n_expected) {
        t = va_arg(ptr, int);
        fprintf(parser.out, "%s", Token_GetName(t));
        printf("%s", Token_GetName(t));
        if(i < n_expected-1) {
            fprintf(parser.out, ", ");
            printf(", ");
        }
        i++;
    }
    va_end(ptr);
    CONSOLE_COLOR_DEFAULT();
}

void ParseError_FunctionFailed(const char * functionName) {
    CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
    int i = 0;
    fputc('\n', parser.out);
    fputc('\n', stdout);
    while(i < parser.trace) {
        fputc(' ', parser.out);
        fputc(' ', stdout);
        i++;
    }
    parser.trace += 1;
    fprintf(parser.out, "%s failed to parse!", functionName);
    printf("%s failed to parse!", functionName);
    CONSOLE_COLOR_DEFAULT();
}

short ParseError_SkipToStatementEnd(int endToken) {
    parser.trace = 0; /* Reset trace for future possible parse errors. */
    fprintf(parser.list, "\n      Attempting to skip tokens until %s to recover from parse error.\n\n", Token_GetName(endToken));
    fprintf(parser.out, "\n\nAttempting to skip tokens until %s to recover from parse error.\n", Token_GetName(endToken));
    printf("\n");
    CONSOLE_COLOR(FG_BRT_YELLOW, BG_DEFAULT);
    printf("Attempting to skip tokens until %s to recover from parse error.", Token_GetName(endToken));
    CONSOLE_COLOR_DEFAULT();
    short err = 0;
    int next = Scanner_NextToken();
    int skip_count = 1;
    short success;
    while(next != SCANEOF && next != END && next != endToken) {
        skip_count++;
        Scanner_Match(next);
        next = Scanner_NextToken();
    }
    if(next == endToken) {
        Scanner_Match(next);
        fprintf(parser.list, "\n      %d Tokens skipped, recovery successful.\n\n", skip_count);
        fprintf(parser.out, "\n\n%d Tokens skipped, recovery successful.\n", skip_count);
        printf("\n");
        CONSOLE_COLOR(FG_BRT_YELLOW, BG_DEFAULT);
        printf("%d Tokens skipped, recovery succesful.", skip_count);
        CONSOLE_COLOR_DEFAULT();
        printf("\n");
    } else {
        fprintf(parser.list, "\n      %d Tokens skipped, %s encountered, recovery unsuccessful!\n", skip_count, Token_GetName(next));
        fprintf(parser.out, "\n\n Tokens skipped, %s encountered, recovery unsuccessful!\n", skip_count, Token_GetName(next));
        printf("\n");
        CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
        printf("%d Tokens skipped, %s encountered, recovery unsuccessful!", skip_count, Token_GetName(next));
        CONSOLE_COLOR_DEFAULT();
        printf("\n");
        err = 1;
    }
    return err; 
}

void Parser_PrintErrorSummary() {
    if(parser.errorCount == 1) {
        fprintf(parser.list, "\n%d Parse Error.", parser.errorCount);
        printf("\n%d Parse Error.", parser.errorCount);
    } else {
        fprintf(parser.list, "\n%d Parse Errors.", parser.errorCount);
        printf("\n%d Parse Errors.", parser.errorCount);
    }
}

int Parser_GetParseErrCount() {
    return parser.errorCount;
}

#pragma endregion parse_errors
/*
-------------------------------
Production rule parse functions
-------------------------------
*/
#pragma region production_rule_parse_functions

short Parse_SystemGoal() {
    short err = Parse_Program();
    if (!err) {
        err = Scanner_Match(SCANEOF);
        if(err) {
            ParseError_MatchFailed(SCANEOF);
        }
    }
    if(err) {
        ParseError_FunctionFailed("SystemGoal");
    }
    return err; 
}

short Parse_Program() {
    short err = Scanner_Match(BEGIN);
    if(!err) {
        Parser_printBufferStatementToOutAndClear();
        err = Parse_StatementList();
        if(!err) {
            err = Scanner_Match(END);
            if(!err) {
                Parser_printBufferStatementToOutAndClear();
            } else {
                ParseError_MatchFailed(END);
            }
        }
    } else {
        ParseError_MatchFailed(BEGIN);
    }
    if(err) {
        ParseError_FunctionFailed("Program");
    }
    return err;
}

short Parse_StatementList() {
    short err = Parse_Statement();
    int next = Scanner_NextToken();
    if(!err) {
        while( (next == ID || next == READ || 
                next == WRITE || next == IF || 
                next == WHILE || next == ERROR ) && err == 0) {                
            err = Parse_Statement();
            next = Scanner_NextToken();
        }
    }
    if(err) {
        ParseError_FunctionFailed("StatementList");
    }
    return err;
}

short Parse_Statement() {
    short err = 0;
    int next = Scanner_NextToken();
    int skip_to_on_fail;
    
    switch (next) {
        case ID: /* Production 3 */
            skip_to_on_fail = SEMICOLON;
            Scanner_Match(ID);
            err = Scanner_Match(ASSIGNOP);
            if(!err) {
                err = Parse_Expression();
                if(!err) {
                    err = Scanner_Match(SEMICOLON);
                    if(err) {
                        ParseError_MatchFailed(SEMICOLON);
                    }
                }
            } else {
                ParseError_MatchFailed(ASSIGNOP);
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case READ: /* Production 4 */
            skip_to_on_fail = SEMICOLON;
            Scanner_Match(READ);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_IDList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        err = Scanner_Match(SEMICOLON);
                        if(err) {
                            ParseError_MatchFailed(SEMICOLON);
                        }
                    } else {
                        ParseError_MatchFailed(RPAREN);
                    }
                }
            } else {
                ParseError_MatchFailed(LPAREN);
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case WRITE: /* Production 5 */
            skip_to_on_fail = SEMICOLON;
            Scanner_Match(WRITE);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_ExpressionList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        err = Scanner_Match(SEMICOLON);
                        if(err) {
                            ParseError_MatchFailed(SEMICOLON);
                        }
                    } else {
                        ParseError_MatchFailed(RPAREN);
                    }
                }
            } else {
                ParseError_MatchFailed(LPAREN);
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case IF: /* Production 6 */
            skip_to_on_fail = ENDIF;
            Scanner_Match(IF);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_Condition();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        Parser_printBufferStatementToOutAndClear();
                        err = Scanner_Match(THEN);
                        if(!err) {
                            Parser_printBufferStatementToOutAndClear();
                            err = Parse_StatementList();
                            if(!err) {
                                err = Parse_IfTail();
                            }
                        } else {
                            ParseError_MatchFailed(THEN);
                        }
                    } else {
                        ParseError_MatchFailed(RPAREN);
                    }
                }
            } else {
                ParseError_MatchFailed(LPAREN);
            }
            break;
        case WHILE: /* Production 9 */
            skip_to_on_fail = ENDWHILE;
            Scanner_Match(WHILE);
            err = Scanner_Match(LPAREN);
            if (!err) {
                err = Parse_Condition();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        Parser_printBufferStatementToOutAndClear();
                        short ntok = Scanner_NextToken();
                        if(ntok != ENDWHILE) {
                            err = Parse_StatementList();
                            if(!err) {
                                err = Scanner_Match(ENDWHILE);
                                if(!err) {
                                    Parser_printBufferStatementToOutAndClear();
                                } else {
                                    ParseError_MatchFailed(ENDWHILE);
                                }
                            }
                        } else {
                            Scanner_Match(ENDWHILE);
                            Parser_printBufferStatementToOutAndClear();
                        }
                    } else {
                        ParseError_MatchFailed(RPAREN);

                    }
                }
            } else {
                ParseError_MatchFailed(LPAREN);
            }
            break;
        case ERROR:
            Scanner_Match(ERROR);
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 5, ID, READ, WRITE, IF, WHILE);
            break;
    }
    if(err) {
        ParseError_FunctionFailed("Statement");
        err = ParseError_SkipToStatementEnd(skip_to_on_fail);
    }
    return err;
}

short Parse_IfTail() {
    int next = Scanner_NextToken();
    short err; 
    switch(next) {
        case ELSE:
            Scanner_Match(ELSE);
            Parser_printBufferStatementToOutAndClear();
            err = Parse_StatementList();
            if(!err) {
                err = Scanner_Match(ENDIF);
                if(!err) { 
                    Parser_printBufferStatementToOutAndClear();
                } else {
                    ParseError_MatchFailed(ENDIF);
                }
            }
            break;
        case ENDIF:
            Scanner_Match(ENDIF);
            Parser_printBufferStatementToOutAndClear();
            break;
        case ERROR:
            Scanner_Match(ERROR);
            err = Parse_IfTail(); /* Recurse on a lexical error to skip invalid chars */
            break;
        default:
            ParseError_NextTokenFailed(next, 2, ELSE, ENDIF);
            err = 1;
    }
    if(err) {
        ParseError_FunctionFailed("IfTail");
    }
    return err; 
}

short Parse_IDList() {
    int next;
    short err = Scanner_Match(ID);
    if(!err) {
        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Scanner_Match(ID);
            if(err) {
                ParseError_MatchFailed(ID);
            }
            next = Scanner_NextToken();
        }
    } else {
        ParseError_MatchFailed(ID);
    }
    if(err) {
        ParseError_FunctionFailed("IDList");
    }
    return err;
} 

short Parse_ExpressionList() {
    short err = Parse_Expression();
    if(!err) {
        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Parse_Expression();
            next = Scanner_NextToken();
        }
    }
    if(err) {
        ParseError_FunctionFailed("ExpressionList");
    }
    return err;
}

short Parse_Expression() {
    short err = Parse_Term();
    if(!err) {
        int next = Scanner_NextToken();
        if(next == PLUSOP || next == MINUSOP) {
            err = Parse_AddOP();
            if(!err) {
                err = Parse_Term();
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Expression");
    }
    return err; 
}

short Parse_Term() {
    short err = Parse_Factor();
    if(!err) {
        int next = Scanner_NextToken();
        if(next == MULTOP || next == DIVOP) {
            err = Parse_MultOP();
            if(!err) {
                err = Parse_Factor();
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Term");
    }
    return err; 
}

short Parse_Factor() {
    int next = Scanner_NextToken();
    short err = 0;
    switch(next) {
        case LPAREN:
            Scanner_Match(LPAREN);
            err = Parse_Expression();
            if(!err) {
                err = Scanner_Match(RPAREN);
            }
            break;
        case MINUSOP:
            Scanner_Match(MINUSOP);
            err = Parse_Factor();
            break;
        case ID:
            Scanner_Match(ID);
            break;
        case INTLITERAL:
            Scanner_Match(INTLITERAL);
            break;
        case ERROR:
            Scanner_Match(ERROR);
            err = Parse_Factor(); /* recurse on lexical error to skip it and keep parsing */
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 4, LPAREN, MINUSOP, ID, INTLITERAL);
    }
    if(err) {
        ParseError_FunctionFailed("Factor");
    }
    return err; 
}

short Parse_AddOP() {;
    int next = Scanner_NextToken();
    short err = 0;
    if(next == PLUSOP) {
        Scanner_Match(PLUSOP);
    } else if(next == MINUSOP) {
        Scanner_Match(MINUSOP);
    } else {
        err = 1;
        ParseError_NextTokenFailed(next, 2, PLUSOP, MINUSOP);
    }
    if(err) {
        ParseError_FunctionFailed("AddOP");
    }
    return err; 
}

short Parse_MultOP() {
    int next = Scanner_NextToken();
    short err = 0;
    if(next == MULTOP) {
        Scanner_Match(MULTOP);
    } else if(next == DIVOP) {
        Scanner_Match(DIVOP);
    } else {
        err = 1;
        ParseError_NextTokenFailed(next, 2, MULTOP, DIVOP);
    }
    if(err) {
        ParseError_FunctionFailed("MultOP");
    }
    return err; 
}

short Parse_Condition(){
    short err = Parse_Addition();
    if(!err) {
        int next = Scanner_NextToken();
        if(next == LESSOP || next == LESSEQUALOP || next == GREATEROP || next == GREATEREQUALOP || next == EQUALOP || next == NOTEQUALOP) {
            err = Parse_RelOP();
            if(!err) {
                err = Parse_Addition();
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Condition");
    }
    return err; 
}

short Parse_Addition() {
    short err = Parse_Multiplication();
    if(!err) {
        int next = Scanner_NextToken();
        if(next == PLUSOP || next == MINUSOP) {
            err = Parse_AddOP();
            if(!err) {
                err = Parse_Multiplication();
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Addition");
    }
    return err;
}

short Parse_Multiplication() {
    short err = Parse_Unary();
    if(!err) {
        int next = Scanner_NextToken();
        if(next == MULTOP || next == DIVOP) {
            err = Parse_MultOP();
            if(!err) {
                err = Parse_Unary();
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Multiplication");
    }
    return err;
}

short Parse_Unary() {
    short err = 0;
    int next = Scanner_NextToken();
    switch(next) {
        case NOTOP:
            Scanner_Match(NOTOP);
            err = Parse_Unary();
            break;
        case MINUSOP:
            Scanner_Match(MINUSOP);
            err = Parse_Unary();
            break;
        case ERROR:
            Scanner_Match(ERROR);
            err = Parse_Unary();
            break;
        default:
            err = Parse_LPrimary();
    }
    if(err) {
        ParseError_FunctionFailed("Unary");
    }
    return err; 
}

short Parse_LPrimary() {
    short err = 0;
    int next = Scanner_NextToken();
    switch(next) {
        case INTLITERAL:
            Scanner_Match(INTLITERAL);
            break;
        case ID:
            Scanner_Match(ID);
            break;
        case LPAREN:
            Scanner_Match(LPAREN);
            err = Parse_Condition();
            if(!err) {
                err = Scanner_Match(RPAREN);
            }
            break;
        case FALSEOP:
            Scanner_Match(FALSEOP);
            break;
        case TRUEOP:
            Scanner_Match(TRUEOP);
            break;
        case NULLOP:
            Scanner_Match(NULLOP);
            break;
        case ERROR:
            Scanner_Match(ERROR);
            err = Parse_LPrimary();
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 6, INTLITERAL, ID, LPAREN, FALSEOP, TRUEOP, NULLOP);
            break;
    }
    if(err) {
        ParseError_FunctionFailed("LPrimary");
    }
    return err;
}

short Parse_RelOP() {
    short err = 0;
    int next = Scanner_NextToken();
    switch(next) {
        case LESSOP:
            Scanner_Match(LESSOP);
            break;
        case LESSEQUALOP:
            Scanner_Match(LESSEQUALOP);
            break;
        case GREATEROP:
            Scanner_Match(GREATEROP);
            break;
        case GREATEREQUALOP:
            Scanner_Match(GREATEREQUALOP);
            break;
        case EQUALOP:
            Scanner_Match(EQUALOP);
            break;
        case NOTEQUALOP:
            Scanner_Match(NOTEQUALOP);
            break;
        case ERROR:
            Scanner_Match(ERROR); /* recurse/skip on lexical error */
            err = Parse_RelOP();
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 6, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP);
            break;
    }
    if(err) {
        ParseError_FunctionFailed("RelOP");
    }
    return err;
}

#pragma endregion production_rule_parse_functions