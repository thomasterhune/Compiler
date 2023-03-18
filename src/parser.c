#include "parser.h"


short Parser_Statement(){
    short result = NO_NEXT_STATEMENT;
    switch (Scanner_NextToken())
    {
    case ID:
        if(Scanner_Match(ID)){
            if(Scanner_Match(ASSIGNOP)){
                if(Parser_Expression){
                    if(Scanner_Match(SEMICOLON)){
                        result = ASSIGNMENT_STATEMENT;
                    }
                }
            }
        }
        break;
    case READ:
        Scanner_Match(LPAREN);
        /*call to id list*/
        Scanner_Match(RPAREN);
        Scanner_Match(SEMICOLON);
        result = READ_STATEMENT;
        break;
    case WRITE:
        Scanner_Match(LPAREN);
        /*call to expression list*/
        Scanner_Match(RPAREN);
        Scanner_Match(SEMICOLON);
        result = WRITE_STATEMENT;
        break;
    case IF:
        Scanner_Match(LPAREN);
        /*call to condition*/
        Scanner_Match(RPAREN);
        Scanner_Match(THEN);
        Parser_StatementList();
        /*call to iftail*/
        result = IF_STATEMENT;
        break;
    case WHILE:
        Scanner_Match(LPAREN);
        /*call to condition*/
        Parser_StatementList();
        Scanner_Match(RPAREN);
        Scanner_Match(ENDWHILE);
        result = WHILE_STATEMENT;
        break;
    default:
        result = NO_NEXT_STATEMENT;
        break;
    }
    return result;
}

short Parser_StatementList(){
    short result = 0;
    do{
        if(Scanner_NextToken())
        result = statement();
    }while(result > 0);
}

short Parser_Expression(){

}

short Parser_ExpressionList(){

}

short Parser_IfTail(){

}

short Parser_Condition(){

}

short Parser_IDList(){

}