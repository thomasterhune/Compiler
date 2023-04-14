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
#include <time.h>
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

void Parser_Load(FILE *out, FILE *list, FILE *temp) {
    parser.out = out;
    parser.list = list;
    parser.temp = temp;
    fgetpos(temp, &(parser.tempstart));
    fflush(temp);
}

void Parser_Init() {
    parser.errorCount = 0;
    parser.trace = 0;
    Parser_clearBuffer(); /* sets buffer, capacity, l_buffer */
    SymbolTable_Init();
}

void Parser_DeInit() {
    parser.out = NULL;
    parser.list = NULL;
    if(parser.buffer != NULL) {
        free(parser.buffer);
    }
    parser.buffer = NULL;
    parser.temp = NULL;
    SymbolTable_DeInit();
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
    /*fprintf(parser.out, "\n\nStatement: %s\n", parser.buffer);*/
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
    /*fputc('\n', parser.out);*/
    fputc('\n', stdout);
    while(i < parser.trace) { /* Print at the same indentation level of a function fail*/
        /*fputc(' ', parser.out);*/
        fputc(' ', stdout);
        i++;
    }
    /*fprintf(parser.out, "\nFailed to match expected token: %s", Token_GetName(expected_token)); */
    printf("Failed to match expected token: %s", Token_GetName(expected_token));
    CONSOLE_COLOR_DEFAULT();
}


void ParseError_NextTokenFailed(int actual_token, int n_expected, ...) {
    parser.errorCount += 1;
    Scanner_PrintParseErrorMessage();
    CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
    int i = 0;
    /*fputc('\n', parser.out);*/
    fputc('\n', stdout);
    while(i < parser.trace) {
        /*fputc(' ', parser.out);*/
        fputc(' ', stdout);
        i++;
    }
    /*fprintf(parser.out, "Next token %s was not one of the expected tokens: ", Token_GetName(actual_token));*/
    printf("Next token %s was not one of the expected tokens: ", Token_GetName(actual_token));

    /* Print the variadic list of expected next token values. */
    va_list ptr;
    va_start(ptr, n_expected);
    int t;
    i = 0;
    while(i < n_expected) {
        t = va_arg(ptr, int);
        /*fprintf(parser.out, "%s", Token_GetName(t));*/
        printf("%s", Token_GetName(t));
        if(i < n_expected-1) {
            /*fprintf(parser.out, ", ");*/
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
    /*fputc('\n', parser.out);*/
    fputc('\n', stdout);
    while(i < parser.trace) {
        /* fputc(' ', parser.out); */
        fputc(' ', stdout);
        i++;
    }
    parser.trace += 1;
    /*fprintf(parser.out, "%s failed to parse!", functionName);*/
    printf("%s failed to parse!", functionName);
    CONSOLE_COLOR_DEFAULT();
}

short ParseError_SkipToStatementEnd(int endToken) {
    parser.trace = 0; /* Reset trace for future possible parse errors. */
    fprintf(parser.list, "\n      Attempting to skip tokens until %s to recover from parse error.\n\n", Token_GetName(endToken));
    /*fprintf(parser.out, "\n\nAttempting to skip tokens until %s to recover from parse error.\n", Token_GetName(endToken));*/
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
        /*fprintf(parser.out, "\n\n%d Tokens skipped, recovery successful.\n", skip_count);*/
        printf("\n");
        CONSOLE_COLOR(FG_BRT_YELLOW, BG_DEFAULT);
        printf("%d Tokens skipped, recovery succesful.", skip_count);
        CONSOLE_COLOR_DEFAULT();
        printf("\n");
    } else {
        fprintf(parser.list, "\n      %d Tokens skipped, %s encountered, recovery unsuccessful!\n", skip_count, Token_GetName(next));
        /*fprintf(parser.out, "\n\n Tokens skipped, %s encountered, recovery unsuccessful!\n", skip_count, Token_GetName(next));*/
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
    printf(":: SystemGoal called\n");
    short err = Parse_Program();
    if (!err) {
        err = Scanner_Match(SCANEOF);
        if(err) {
            ParseError_MatchFailed(SCANEOF);
        } else {
            Parse_ActionFinish();
        }
    }
    if(err) {
        ParseError_FunctionFailed("SystemGoal");
    }
    return err; 
}

/* GOOD */
short Parse_Program() {
    Parse_ActionStart();
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

/* GOOD */
short Parse_StatementList() {
    printf("\n :: StatementList called\n");
    short err = Parse_Statement();
    int next = Scanner_NextToken();
    if(!err) {
        while( (next == ID || next == READ || 
                next == WRITE || next == IF || 
                next == WHILE || next == LEX_ERROR ) && err == 0) {                
            err = Parse_Statement();
            next = Scanner_NextToken();
        }
    }
    if(err) {
        ParseError_FunctionFailed("StatementList");
    }
    printf("\n   StatementList returning ::\n");
    return err;
}

/* ID, Read good, need if, while, lex error */
short Parse_Statement() {
    printf("\n :: Statement called\n");
    short err = 0;
    int next = Scanner_NextToken();
    int skip_to_on_fail;

    EXPR_RECORD source;
    EXPR_RECORD target;
    
    switch (next) {
        case ID: /* Production 3 */
            skip_to_on_fail = SEMICOLON;
            err = Parse_Ident(&target);
            if(!err) {
                err = Scanner_Match(ASSIGNOP);
                    if(!err) {
                        err = Parse_Expression(&source);
                        if(!err) {
                            printf("\n  STATEMENT CALLING ACTION ASSIGN ! \n");
                            Parse_ActionAssign(&target, &source);
                            err = Scanner_Match(SEMICOLON);
                            if(err) {
                                ParseError_MatchFailed(SEMICOLON);
                            }
                            free(target.reference);
                            free(source.reference);
                        }
                    } else {
                        ParseError_MatchFailed(ASSIGNOP);
                }
                            
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
                err = Parse_Condition(&source);
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
                err = Parse_Condition(&source);
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
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR);
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
    printf("\n   Statement returning ::\n");
    return err;
}

/* NOT STARTED */
short Parse_IfTail() {
    printf(":: IfTail called\n");
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
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR);
            err = Parse_IfTail(); /* Recurse on a lexical error to skip invalid chars */
            break;
        default:
            ParseError_NextTokenFailed(next, 2, ELSE, ENDIF);
            err = 1;
    }
    if(err) {
        ParseError_FunctionFailed("IfTail");
    }
    printf("\n   IfTail returning ::\n");
    return err; 
}

/* GOOD */
short Parse_IDList() {
    printf(":: ParseIDList called\n");
    int next;
    short err = Scanner_Match(ID);
    if(!err) {
        
        EXPR_RECORD expr;
        expr.reference = Scanner_GetBufferCopy();
        expr.type = EXPR_ID;
        SymbolTable_CheckID(expr.reference, parser.out);
        Parse_ActionReadID(&expr);
        free(expr.reference);
        
        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Scanner_Match(ID);
            if(err) {
                ParseError_MatchFailed(ID);
            } else {
                expr.reference = Scanner_GetBufferCopy();
                SymbolTable_CheckID(expr.reference, parser.out);
                Parse_ActionReadID(&expr);
            }
            next = Scanner_NextToken();
        }
    } else {
        ParseError_MatchFailed(ID);
    }
    if(err) {
        ParseError_FunctionFailed("IDList");
    }
    printf("\n   ParseIDList returning ::\n");
    return err;
} 

/* GOOD */
short Parse_ExpressionList() {
    printf("\n:: ParseExpressionList called\n");
    EXPR_RECORD expr;
    short err = Parse_Expression(&expr);
    if(!err) {
        Parse_ActionWriteExpr(&expr);
        free(expr.reference);

        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Parse_Expression(&expr);
            Parse_ActionWriteExpr(&expr);
            free(expr.reference);

            next = Scanner_NextToken();
        }
    }
    if(err) {
        ParseError_FunctionFailed("ExpressionList");
    }
    printf("\n   ParseExpressionList returning ::\n");
    return err;
}

/* Probably Good */
short Parse_Expression(EXPR_RECORD * expr_record) {
    printf("\n:: ParseExpression called\n");
    OP_RECORD op_record;
    EXPR_RECORD left_operand;
    EXPR_RECORD right_operand;
    short err = Parse_Term(&left_operand);
    if(!err) {
        int next = Scanner_NextToken();
        if(next == PLUSOP || next == MINUSOP) {
            err = Parse_AddOP(&op_record);
            if(!err) {
                err = Parse_Term(&right_operand);
                if(!err) {
                    *expr_record = Parse_ActionGenInfix(&left_operand, &op_record, &right_operand);
                    free(left_operand.reference);
                    free(right_operand.reference);
                }
            }
        } else {
            *expr_record = left_operand;
        }
    }
    if(err) {
        ParseError_FunctionFailed("Expression");
    }
    printf("\n   ParseExpression returning ::\n");
    return err; 
}

short Parse_Term(EXPR_RECORD * expr_record) {
    printf("\n:: ParseTerm called\n");
    EXPR_RECORD left_operand;
    EXPR_RECORD right_operand;
    OP_RECORD op_record;
    short err = Parse_Factor(&left_operand);
    if(!err) {
        int next = Scanner_NextToken();
        if(next == MULTOP || next == DIVOP) {
            err = Parse_MultOP(&op_record);
            if(!err) {
                err = Parse_Factor(&right_operand);
                if(!err) {
                    *expr_record = Parse_ActionGenInfix(&left_operand, &op_record, &right_operand);
                }
                free(left_operand.reference);
                free(right_operand.reference);
            }
        } else {
            *expr_record = left_operand;
        }
    }
    if(err) {
        ParseError_FunctionFailed("Term");
    }
    printf("\n   ParseTerm returning ::\n");
    return err; 
}

/* NOT DONE - only case ID done */

short Parse_Factor(EXPR_RECORD * expr_record) {
    printf("\n:: ParseFactor called\n");
    EXPR_RECORD left_operand;
    EXPR_RECORD right_operand;
    OP_RECORD op_record;
    int next = Scanner_NextToken();
    short err = 0;
    switch(next) {
        case LPAREN:
            Scanner_Match(LPAREN);
            err = Parse_Expression(expr_record);
            if(!err) {
                err = Scanner_Match(RPAREN);
            }
            break;
        case MINUSOP:
            Scanner_Match(MINUSOP);
            err = Parse_Factor(expr_record);
            break;
        case ID:
            err = Parse_Ident(expr_record);
            break;
        case INTLITERAL:
            Scanner_Match(INTLITERAL);
            *expr_record = Parse_ActionProcessLiteral();
            break;
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR);
            err = Parse_Factor(expr_record); /* recurse on lexical error to skip it and keep parsing */
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 4, LPAREN, MINUSOP, ID, INTLITERAL);
    }
    if(err) {
        ParseError_FunctionFailed("Factor");
    }
    printf("\n   ParseFactor returning ::\n");
    return err; 
}

short Parse_AddOP(OP_RECORD * op_record) {
    printf("\n:: AddOp called\n");
    int next = Scanner_NextToken();
    short err = 0;
    if(next == PLUSOP) {
        Scanner_Match(PLUSOP);
        Parse_ActionProcessOp(op_record);
    } else if(next == MINUSOP) {
        Scanner_Match(MINUSOP);
        Parse_ActionProcessOp(op_record);
    } else {
        err = 1;
        ParseError_NextTokenFailed(next, 2, PLUSOP, MINUSOP);
    }
    if(err) {
        ParseError_FunctionFailed("AddOP");
    }
    printf("\n   AddOp returning ::\n");
    return err; 
}

short Parse_MultOP(OP_RECORD * op_record) {
    printf("\n:: ParseMultOp called\n");
    int next = Scanner_NextToken();
    short err = 0;
    if(next == MULTOP) {
        Scanner_Match(MULTOP);
        Parse_ActionProcessOp(op_record);
    } else if(next == DIVOP) {
        Scanner_Match(DIVOP);
        Parse_ActionProcessOp(op_record);
    } else {
        err = 1;
        ParseError_NextTokenFailed(next, 2, MULTOP, DIVOP);
    }
    if(err) {
        ParseError_FunctionFailed("MultOP");
    }
    printf("\n   ParseMultOp returning ::\n");
    return err; 
}

/* In progress */

short Parse_Condition(EXPR_RECORD * expr_record){
    printf("\n:: ParseCondition called\n");
    EXPR_RECORD left_operand;
    EXPR_RECORD right_operand;
    OP_RECORD op_record;
    short err = Parse_Addition(&left_operand);
    if(!err) {
        int next = Scanner_NextToken();
        if(next == LESSOP || next == LESSEQUALOP || next == GREATEROP || next == GREATEREQUALOP || next == EQUALOP || next == NOTEQUALOP) {
            err = Parse_RelOP(&op_record);
            if(!err) {
                err = Parse_Addition(&right_operand);
                if(!err) {
                    left_operand = Parse_ActionGenInfix(&left_operand, &op_record, &right_operand);
                    free(right_operand.reference);
                }
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Condition");
    } else {
        *expr_record = left_operand;
    }
    printf("\n   ParseCondition returning ::\n");
    return err; 
}

short Parse_Addition(EXPR_RECORD * expr_record) {
    printf("\n:: ParseAddition called\n");
    EXPR_RECORD left_operand, right_operand;
    OP_RECORD op_record;
    short err = Parse_Multiplication(&left_operand);
    if(!err) {
        int next = Scanner_NextToken();
        if(next == PLUSOP || next == MINUSOP) {
            err = Parse_AddOP(&op_record);
            if(!err) {
                err = Parse_Multiplication(&right_operand);
                if(!err) {
                    left_operand = Parse_ActionGenInfix(&left_operand, &op_record, &right_operand);
                    free(right_operand.reference);
                }
            }
        }
    }
    if(err) {
         ParseError_FunctionFailed("Addition");
    } else {
        *expr_record = left_operand;
    }
    printf("\n   ParseAddition returning ::\n");
    return err;
}

short Parse_Multiplication(EXPR_RECORD * expr_record) {
    printf("\n :: ParseMultiplication called\n");
    EXPR_RECORD left_operand, right_operand;
    OP_RECORD op_record;
    short err = Parse_Unary(&left_operand);
    if(!err) {
        int next = Scanner_NextToken();
        if(next == MULTOP || next == DIVOP) {
            err = Parse_MultOP(&op_record);
            if(!err) {
                err = Parse_Unary(&right_operand);
                if(!err) {
                    left_operand = Parse_ActionGenInfix(&left_operand, &op_record, &right_operand);
                    free(right_operand.reference);
                }
            }
        }
    }
    if(err) {
        ParseError_FunctionFailed("Multiplication");
    } else {
        *expr_record = left_operand;
    }
    printf("\n   ParseMultiplication returning ::\n");
    return err;
}

short Parse_Unary(EXPR_RECORD * expr_record) {
     printf("\n :: ParseUnary called\n");
    short err = 0;
    int next = Scanner_NextToken();
    EXPR_RECORD copy;
    switch(next) {
        case NOTOP:
            Scanner_Match(NOTOP);
            err = Parse_Unary(expr_record);
            copy = Parse_PrependedCopy(expr_record, "!");
            free(expr_record->reference);
            *expr_record = copy;
            break;
        case MINUSOP:
            Scanner_Match(MINUSOP);
            err = Parse_Unary(expr_record);
            copy = Parse_PrependedCopy(expr_record, "-");
            free(expr_record->reference);
            *expr_record = copy;
            break;
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR);
            err = Parse_Unary(expr_record);
            break;
        default:
            err = Parse_LPrimary(expr_record);
    }
    if(err) {
        ParseError_FunctionFailed("Unary");
    }
    printf("\n   ParseUnary returning ::\n");
    return err; 
}

short Parse_LPrimary(EXPR_RECORD * expr_record) {
     printf("\n :: ParseLPprimary called\n");
    short err = 0;
    int next = Scanner_NextToken();
    switch(next) {
        case INTLITERAL:
            Scanner_Match(INTLITERAL);
            *expr_record = Parse_ActionProcessLiteral();
            break;
        case ID:
            Scanner_Match(ID);
            *expr_record = Parse_ActionProcessID();
            break;
        case LPAREN:
            Scanner_Match(LPAREN);
            err = Parse_Condition(expr_record);
            if(!err) {
                err = Scanner_Match(RPAREN);
            }
            break;
        case FALSEOP:
            Scanner_Match(FALSEOP);
            *expr_record = Parse_ActionProcessTokenAlias(FALSEOP);
            break;
        case TRUEOP:
            Scanner_Match(TRUEOP);
            *expr_record = Parse_ActionProcessTokenAlias(TRUEOP);
            break;
        case NULLOP:
            Scanner_Match(NULLOP);
            *expr_record = Parse_ActionProcessTokenAlias(NULLOP);
            break;
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR);
            err = Parse_LPrimary(expr_record);
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 6, INTLITERAL, ID, LPAREN, FALSEOP, TRUEOP, NULLOP);
            break;
    }
    if(err) {
        ParseError_FunctionFailed("LPrimary");
    }
    printf("\n   ParseLPprimary returning ::\n");
    return err;
}

short Parse_RelOP(OP_RECORD * op_record) {
    printf("\n :: ParseRelOp called\n");
    short err = 0;
    int next = Scanner_NextToken();
    switch(next) {
        case LESSOP:
            Scanner_Match(LESSOP);
            *op_record = Parse_ActionProcessOp();
            break;
        case LESSEQUALOP:
            Scanner_Match(LESSEQUALOP);
            *op_record = Parse_ActionProcessOp();
            break;
        case GREATEROP:
            Scanner_Match(GREATEROP);
            *op_record = Parse_ActionProcessOp();
            break;
        case GREATEREQUALOP:
            Scanner_Match(GREATEREQUALOP);
            *op_record = Parse_ActionProcessOp();
            break;
        case EQUALOP:
            Scanner_Match(EQUALOP);
            *op_record = Parse_ActionProcessOp();
            op_record->data[1] = '=';
            op_record->data[2] = '\0';
            break;
        case NOTEQUALOP:
            Scanner_Match(NOTEQUALOP);
            *op_record = Parse_ActionProcessOp();
            op_record->data[0] = '!';
            op_record->data[1] = '=';
            op_record->data[2] = '\0';
            break;
        case LEX_ERROR:
            Scanner_Match(LEX_ERROR); /* recurse/skip on lexical error */
            err = Parse_RelOP(op_record);
            break;
        default:
            err = 1;
            ParseError_NextTokenFailed(next, 6, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP);
            break;
    }
    if(err) {
        ParseError_FunctionFailed("RelOP");
    }
    printf("\n   ParseRelOp returning ::\n");
    return err;
}

short Parse_Ident(EXPR_RECORD * expr_record) {
    printf("\n :: ParseIdent called\n");
    short err = 0;
    int next = Scanner_NextToken();
    if(next == ID) {
        Scanner_Match(ID);
        *expr_record = Parse_ActionProcessID();
        fflush(parser.out); fflush(stdout);
    } else {
        err = 1;
    }
    if(err) {
        ParseError_FunctionFailed("Ident");
    }
    printf("\n   ParseIdent returning ::\n");
    return err;
}
#pragma endregion production_rule_parse_functions

#pragma region action_functions

void Parse_ActionStart() {
    printf("\n >> ACTION START CALLED! ");
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);     /*get local time*/


    fprintf(parser.out, "#include <stdio.h>\n\n");
    fprintf(parser.out, "/*%s*/\n\n", asctime(info));
    fprintf(parser.out, "int main(int argc, char ** argv) {\n");
}

void Parse_ActionFinish() {
    printf("\n >> ACTION FINISH CALLED! ");
    /*concatenate files together*/
    
    int  c;
    fsetpos(parser.temp, &(parser.tempstart));
    c = fgetc(parser.temp);
    printf(" \n%d <-- next char #", c);
    
    while(c != EOF)
    {
        fputc( c , parser.out);
        c = fgetc(parser.temp);

    }
    fprintf(parser.out, "\n\treturn 0;\n}\n/*PROGRAM COMPILED WITH %d ERRORS.*/\n", Parser_GetParseErrCount());
}

void Parse_ActionAssign(EXPR_RECORD * target, EXPR_RECORD * source) {
    printf("\n >> ACTION ASSIGN CALLED! %s %s", target->reference, source->reference);
    SymbolTable_Generate(parser.temp, target->reference, " = ", source->reference, "", "");
    fflush(stdout);
    printf("\n >> ACTION ASSIGN RETURNING! %s %s", target->reference, source->reference);
}

OP_RECORD Parse_ActionProcessOp() {
    OP_RECORD op_rec;
    printf("\n >> ACTION PROCESS OP CALLED! ");
    char * scan_buff = Scanner_GetBuffer();
    int l_buffer = * Scanner_GetLBuffPointer();
    int i = 0;
    for(i = 0; i < l_buffer && i < 3; i++) {
        op_rec.data[i] = scan_buff[i];
    }
    op_rec.data[i] = '\0';
    printf("\n  `-->> op: %s", op_rec.data);
    printf("\n >> ACTION PROCESS OP RETURNING!");
    return op_rec;
}

EXPR_RECORD Parse_ActionProcessID() {
    printf("\n >> ACTION PROCESS ID CALLED! ");
    EXPR_RECORD nexpr;
    nexpr.type = EXPR_ID;
    nexpr.reference = Scanner_GetBufferCopy();
    printf("\n  `-->>  reference: %s", nexpr.reference);
    SymbolTable_CheckID(nexpr.reference, parser.out);
    printf("\n ... returning from action process ID");

    SymbolTable_DBPrintAll();
    fflush(stdout);
    printf("\n >> ACTION PROCESS ID RETURNING! ");
    return nexpr;
}

EXPR_RECORD Parse_ActionGenInfix(EXPR_RECORD * left_side, OP_RECORD *op_record, EXPR_RECORD *right_side) {
    printf("\n >> ACTION GEN INFIX CALLED! ");
    EXPR_RECORD result;
    result.type = EXPR_TEMP;
    result.reference = SymbolTable_GetTemp();
    printf("\n >> ACTION GEN INFIX TEMP STRING VAL %s ", result.reference);
    SymbolTable_CheckID(result.reference, parser.out);
    SymbolTable_Generate(parser.temp, result.reference, "=", left_side->reference, op_record->data, right_side->reference);
    printf("\n >> ACTION GEN INFIX RETURNING! ");
    return result;
}

EXPR_RECORD Parse_ActionProcessLiteral() {
    printf("\n >> ACTION LITERAL CALLED! ");
    EXPR_RECORD exp_rec;
    exp_rec.type = EXPR_INTLIT;
    exp_rec.reference = Scanner_GetBufferCopy();
    printf("\n  `-->>  reference: %s", exp_rec.reference);
    printf("\n >> ACTION LITERAL RETURNING! ");
    return exp_rec;
}

EXPR_RECORD Parse_ActionProcessTokenAlias(int token) {
    EXPR_RECORD expr_record;
    expr_record.type = EXPR_INTLIT;
    expr_record.reference = Token_CTranslateMalloc(token);
    return expr_record;
}

void Parse_ActionReadID(EXPR_RECORD * target) {
    fprintf(parser.temp, "\n\tscanf(\"%%d\", &%s);",target->reference);
}

void Parse_ActionWriteExpr(EXPR_RECORD * target) {
    if(target->type == EXPR_ID) {
        fprintf(parser.temp, "\n\tprintf(\"\\n%s = %%d\", %s);",target->reference, target->reference);
    } else {
        fprintf(parser.temp, "\n\tprintf(\"\\n%%d\", %s)", target->reference);
    }
}

EXPR_RECORD Parse_PrependedCopy(EXPR_RECORD * source, char * prepend) {
    EXPR_RECORD copy;
    size_t len = strlen(source->reference);
    size_t prp_len = strlen(prepend);
    copy.reference = malloc( (len+prp_len+1)*sizeof(char));
    strcpy(copy.reference, prepend);
    strcat(copy.reference, source->reference);
    copy.reference[len+prp_len] = '\0';
    copy.type = source->type;
    return copy;
}

#pragma endregion action_functions