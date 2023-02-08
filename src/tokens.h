#ifndef tokens_h
#define tokens_h

enum TOKEN {
    BEGIN=0, END, READ, WRITE, IF, THEN, ELSE, ENDIF, WHILE, ENDWHILE, ID, INTLITERAL, FALSEOP, TRUEOP, NULLOP, LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP, MULTOP,DIVOP, NOTOP, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP, SCANEOF, ERROR
};

/*! Token_GetName gets a character string representing a token. 

    \param id The token ENUM to retrieve as a malloced string.
    \return const char* A string from a lookup table, e.g, "BEGIN". If the param is not a valid token, then it returns "NULL".
    \author klm127
    \date 2/7/2023
    \note Not Covered By Unit Tests
*/
const char * Tokens_GetName(int id);



#endif