#ifndef tokens_h
#define tokens_h
/*!
    \file tokens.h
    \brief Token functions declarations.

    The tokensMap maps a given token to a constant string, which is used by Token_GetName() to get the name of a token. The index of a token string in the tokensMap is the same as it's enumerated value. E.G, BEGIN is value 0 and "BEGIN" is at position 0 in the tokensMap array. 

    This file also contains declarations for TokenCatch methods, which are no longer used. In an earlier version of the program, a TokenCatch wrapped a given token with related data and was memory-allocated. The current version does not use TokenCatch, but it is retained here in case we need it for future parsing features. 

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/

#include <stdlib.h>

enum TOKEN {
    BEGIN=0, END, READ, WRITE, IF, THEN, ELSE, ENDIF, WHILE, ENDWHILE, ID, INTLITERAL, FALSEOP, TRUEOP, NULLOP, LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP, MULTOP,DIVOP, NOTOP, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP, SCANEOF, ERROR
};

/*! Token_GetName gets a character string representing a token. 

    \param id The token ENUM to retrieve.
    \return const char* A string from a lookup table, e.g, "BEGIN". If the param is not a valid token, then it returns "NULL".
    \author klm127
    \date 2/7/2023
    \note Covered By Unit Tests
*/
const char * Token_GetName(int id);

#endif