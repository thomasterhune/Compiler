#include "parse.h"
#include <stdio.h>
#include "tokens.h"
#include <string.h>
#include "scan.h"

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
    Parser_clearBuffer();
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
Scanner buffer
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
    while(end > parser.capacity) {
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
    parser.buffer[parser.l_buffer] = '\0';
    fprintf(parser.out, "\n\nStatement: %s\n", parser.buffer);
    parser.l_buffer = 0;
}


#pragma endregion buffer


/*
-------------------------------
Production rule parse functions
-------------------------------
*/
#pragma region production_rule_parse_functions

short Parse_SystemGoal() {
    printf("\n SystemGoal called.");
    short is_err = Parse_Program();
    if (!is_err) {
        is_err = Scanner_Match(SCANEOF);
    }
    return is_err; 
}

short Parse_Program() {
    printf("\n Parse_Program called.");
    short begin_err = Scanner_Match(BEGIN);
    Parser_printBufferStatementToOutAndClear();
    short stmlist_err = Parse_StatementList();
    short end_err = Scanner_Match(END);
    Parser_printBufferStatementToOutAndClear();
    printf("\n Parse_Program returned with err: %d", begin_err | stmlist_err | end_err);
    return begin_err | stmlist_err | end_err;
}

short Parse_StatementList() {
    printf("\n Parse_StatementList called.");
    int next;
    short err = Parse_Statement();
    if(!err) {
        int next = Scanner_NextToken();
        while( (next == ID || next == READ || 
                next == WRITE || next == IF || 
                next == WHILE) && !err) {                
            err = Parse_Statement();
            next = Scanner_NextToken();
        }
    }
    printf("\n Parse_StatementList returned with err: %d", err);
    return err;
}

short Parse_Statement() {
    printf("\n Parse_Statement called.");
    short err = 0;
    int next = Scanner_NextToken();
    printf("\n  Statement next: %s", Token_GetName(next));
    
    switch (next) {
        case ID: /* Production 3 */
            Scanner_Match(ID);
            err = Scanner_Match(ASSIGNOP);
            if(!err) {
                err = Parse_Expression();
                if(!err) {
                    err = Scanner_Match(SEMICOLON);
                }
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case READ: /* Production 4 */
            Scanner_Match(READ);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_IDList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        err = Scanner_Match(SEMICOLON);
                    }
                }
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case WRITE: /* Production 5 */
            Scanner_Match(WRITE);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_ExpressionList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        err = Scanner_Match(SEMICOLON);
                    }
                }
            }
            Parser_printBufferStatementToOutAndClear();
            break;
        case IF: /* Production 6 */
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
                        }
                    }
                }
            }
            break;
        case WHILE: /* Production 9 */
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
                            err = Scanner_Match(ENDWHILE);
                            if(!err) {
                                Parser_printBufferStatementToOutAndClear();
                            }
                        } else {
                            Scanner_Match(ENDWHILE);
                            Parser_printBufferStatementToOutAndClear();
                        }
                    }
                }

            }
            break;
        case ERROR: /* Handle lexical error */
            /* TODO: lexical error... technically unreachable to do StatementList logic.... figure this one out */
            break;
        default:
            err = 1;
            break;
    }
    printf("\n Parse_Statement returned with err: %d", err);
    return err;
}

short Parse_IfTail() {
    printf("\n Parse_IfTail called.");
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
                }
            }
            break;
        case ENDIF:
            Scanner_Match(ENDIF);
            Parser_printBufferStatementToOutAndClear();
            break;
        case ERROR:
            /* TODO: Handle lexical error */
            break;
        default:
            err = 1;
    }
    printf("\n Parse_IfTail returned with err: %d", err);
    return err; 
}

short Parse_IDList() {
    printf("\n Parse_IDList called.");
    int next;
    short err = Scanner_Match(ID);
    if(!err) {
        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Scanner_Match(ID);
            next = Scanner_NextToken();
        }
    }
    printf("\n Parse_IDList returned with err: %d", err);
    return err;
} 

short Parse_ExpressionList() {
    printf("\n Parse_ExpressionList called.");
    short err = Parse_Expression();
    if(!err) {
        int next = Scanner_NextToken();
        while(next == COMMA && !err) {
            Scanner_Match(COMMA);
            err = Parse_Expression(ID);
            next = Scanner_NextToken();
        }
    }
    printf("\n Parse_ExpressionList returned with err: %d", err);
    return err;
}

short Parse_Expression() {
    printf("\n Parse_Expression called.");
    short err = Parse_Term();
    if(!err) {
        int next = Scanner_NextToken();
        printf("\n  Next token in expression: %s", Token_GetName(next));
        if(next == PLUSOP || next == MINUSOP) {
            err = Parse_AddOP();
            if(!err) {
                err = Parse_Term();
            }
        }
    }
    printf("\n Parse_Expression returned with err: %d", err);
    return err; 
}

short Parse_Term() {
    printf("\n Parse_Term called.");
    short err = Parse_Factor();
    if(!err) {
        int next = Scanner_NextToken();
        printf("\n  Next token in term: %s", Token_GetName(next));
        if(next == MULTOP || next == DIVOP) {
            err = Parse_MultOP();
            if(!err) {
                err = Parse_Factor();
            }
        }
    }
    printf("\n Parse_Term returned with err: %d", err);
    return err; 
}

short Parse_Factor() {
    printf("\n Parse_Factor called.");
    int next = Scanner_NextToken();
    printf("\n  Next token in factor: %s", Token_GetName(next));
    short err = 0;
    switch(next) {
        case LPAREN:
            printf("\n  Parse_Factor, found LPAREN");
            Scanner_Match(LPAREN);
            err = Parse_Expression();
            if(!err) {
                err = Scanner_Match(RPAREN);
            }
            break;
        case MINUSOP:
            printf("\n  Parse_Factor, found MINUSOP");
            Scanner_Match(MINUSOP);
            err = Parse_Factor();
            break;
        case ID:
            printf("\n  Parse_Factor, found ID");
            Scanner_Match(ID);
            break;
        case INTLITERAL:
            printf("\n  Parse_Factor, found INTLITERAL");
            Scanner_Match(INTLITERAL);
            break;
        case ERROR:
            err = 1;
            /*TODO: Lexical error*/
            break;
        default:
            err = 1;
            /*TODO: Syntax error */
    }
    printf("\n Parse_Factor returned with err: %d", err);
    return err; 
}

short Parse_AddOP() {
    printf("\n Parse_AddOP called.");
    int next = Scanner_NextToken();
    short err = 0;
    if(next == PLUSOP) {
        Scanner_Match(PLUSOP);
    } else if(next == MINUSOP) {
        Scanner_Match(MINUSOP);
    } else {
        err = 1;
    }
    printf("\n Parse_AddOP returned with err: %d", err);
    return err; 
}

short Parse_MultOP() {
    printf("\n Parse_MultOP called.");
    int next = Scanner_NextToken();
    short err = 0;
    if(next == MULTOP) {
        Scanner_Match(MULTOP);
    } else if(next == DIVOP) {
        Scanner_Match(DIVOP);
    } else {
        err = 1;
    }
    printf("\n Parse_MultOP returned with err: %d", err);
    return err; 
}

short Parse_Condition(){
    printf("\n Parse_Condition called.");
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
    printf("\n Parse_Condition returned with err: %d", err);
    return err; 
}

short Parse_Addition() {
    printf("\n Parse_Addition called.");
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
    printf("\n Parse_Addition returned with err: %d", err);
    return err;
}

short Parse_Multiplication() {
    printf("\n Parse_Multiplication called.");
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
    printf("\n Parse_Multiplication returned with err: %d", err);
    return err;
}

short Parse_Unary() {
    printf("\n Parse_Unary called.");
    short err = 0;
    int next = Scanner_NextToken();
    printf("\n  Parse_Unary next: %s", Token_GetName(next));
    switch(next) {
        case NOTOP:
            Scanner_Match(NOTOP);
            err = Parse_Unary();
            break;
        case MINUSOP:
            Scanner_Match(MINUSOP);
            err = Parse_Unary();
            break;
        default:
            err = Parse_LPrimary();
    }
    printf("\n Parse_Unary returned with err: %d", err);
    return err; 
}

short Parse_LPrimary() {
    printf("\n Parse_LPrimary called.");
    short err = 0;
    int next = Scanner_NextToken();
    printf("\n  Parse_LPrimary next: %s", Token_GetName(next));
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
        default:
            err = 1;
            break;
    }
    printf("\n Parse_LPrimary returned with err: %d", err);
    return err;
}

short Parse_RelOP() {
    printf("\n Parse_RelOP called.");
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
        default:
            err = 1;
            break;
    }
    printf("\n Parse_RelOP returned with err: %d", err);
    return err;
}

#pragma endregion production_rule_parse_functions