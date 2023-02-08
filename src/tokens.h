#ifndef tokens_h
#define tokens_h

enum TOKEN {
    BEGIN=0, END, READ, WRITE, IF, THEN, ELSE, ENDIF, WHILE, ENDWHILE, ID, INTLITERAL, FALSEOP, TRUEOP, NULLOP, LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP, MULTOP,DIVOP, NOTOP, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP, SCANEOF, ERROR
};

/*! Token_GetName gets a character string representing a token. 

    \param id The token ENUM to retrieve.
    \return const char* A string from a lookup table, e.g, "BEGIN". If the param is not a valid token, then it returns "NULL".
    \author klm127
    \date 2/7/2023
    \note Not Covered By Unit Tests
*/
const char * Token_GetName(int id);

/*! Token_Recognize recognizes a token from a string parameter and returns the corresponding token id.
    \param word The string to identify.
    \param length The length of characters to parse.
    \pre The token is only alphanumeric characters and has already been validated not to contain a comment. (Two '--' chars.)
    \pre The token does not contain any valid operators. (It will be recognized as an identifier if so.)
    \return int a integer within the TOKEN enum; ERROR if it was not a valid word.
*/
int Token_RecognizeKeyword(char * word, int length);

#endif