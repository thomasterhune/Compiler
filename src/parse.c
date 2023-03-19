#include "parse.h"
#include <stdio.h>
#include "tokens.h"

short Parse_SystemGoal(){
    printf("\nSystemGoal called.");
    short is_err = Parse_Program();
    if (!is_err) {
        is_err = Scanner_Match(SCANEOF);
    }
    return is_err; 
}

short Parse_Program() {
    printf("\nProgram called.");
    short begin_err = Scanner_Match(BEGIN);
    short stmlist_err = Parse_StatementList();
    short end_err = Scanner_Match(END);
    return begin_err | stmlist_err | end_err;
}

short Parse_StatementList(){
    printf("\nStatement list called.");
    short err = Parse_Statement();
}


short Parse_Statement(){
    printf("\nStatement called.");
    short err = 0;

    printf("\nStatement next token: %s", Token_GetName(Scanner_NextToken()));
    
    switch (Scanner_NextToken()) {
        case ID: /* Production 3 */
            Scanner_Match(ID);
            err = Scanner_Match(ASSIGNOP);
            if(!err) {
                err = Scanner_Match(ASSIGNOP);
                if(!err) {
                    err = Parse_Expression();
                    if(!err) {
                        err = Scanner_Match(SEMICOLON);
                    }
                }
            }
            break;
        case READ: /* Production 4 */
            Scanner_Match(READ);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_IDList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        Scanner_Match(SEMICOLON);
                    }
                }
            }
            break;
        case WRITE: /* Production 5 */
            Scanner_Match(WRITE);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_ExpressionList();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        Scanner_Match(SEMICOLON);
                    }
                }
            }
            break;
        case IF: /* Production 6 */
            Scanner_Match(IF);
            err = Scanner_Match(LPAREN);
            if(!err) {
                err = Parse_Condition();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        err = Scanner_Match(THEN);
                        if(!err) {
                            err = Parse_StatementList();
                            if(!err) {
                                err = Parse_IfTail();
                                if(!err) {
                                    err = Scanner_Match(SEMICOLON);
                                }
                            }
                        }
                    }
                }
            }
            Parse_StatementList();
            break;
        case WHILE: /* Production 9 */
            Scanner_Match(WHILE);
            err = Scanner_Match(LPAREN);
            if (!err) {
                err = Parse_Condition();
                if(!err) {
                    err = Scanner_Match(RPAREN);
                    if(!err) {
                        short ntok = Scanner_NextToken();
                        if(ntok != ENDWHILE) {
                            err = Parse_StatementList();
                        } else {
                            Scanner_Match(ENDWHILE);
                        }
                    }
                }

            }
            break;
        default:
            err = 1;
            break;
    }
    return err;
}




short Parse_Expression(){

}

short Parse_ExpressionList(){

}

short Parse_IfTail(){

}

short Parse_Condition(){

}

short Parse_IDList(){

} 
