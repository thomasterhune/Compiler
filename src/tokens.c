/*!
    \file tokens.c
    \brief Token map and related functions

    The tokensMap maps a given token to a constant string, which is used by Token_GetName() to get the name of a token. The index of a token string in the tokensMap is the same as it's enumerated value. E.G, BEGIN is value 0 and "BEGIN" is at position 0 in the tokensMap array. 

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/
/*
    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 
*/

#include "tokens.h"
#include <string.h>
#include <stdio.h>

/*!
    TokensMap maps each token to the corresponding string.
    \warning If you change the order in the enum, you must also change the order in this map!

*/
const char *tokensMap[] = {
    [BEGIN] = "BEGIN",
    [END] = "END",
    [READ] = "READ",
    [WRITE] = "WRITE",
    [IF] = "IF",
    [THEN] = "THEN",
    [ELSE] = "ELSE",
    [ENDIF] = "ENDIF",
    [WHILE] = "WHILE",
    [ENDWHILE] = "ENDWHILE",
    [ID] = "ID",
    [INTLITERAL] = "INTLITERAL",
    [FALSEOP] = "FALSEOP",
    [TRUEOP] = "TRUEOP",
    [NULLOP] = "NULLOP",

    [LPAREN] = "LPAREN",
    [RPAREN] = "RPAREN",
    [SEMICOLON] = "SEMICOLON",
    [COMMA] = "COMMA",
    [ASSIGNOP] = "ASSIGNOP",
    [PLUSOP] = "PLUSOP",
    [MINUSOP] = "MINUSOP",
    [MULTOP] = "MULTOP",
    [DIVOP] = "DIVOP",
    [NOTOP] = "NOTOP",
    [LESSOP] = "LESSOP",
    [LESSEQUALOP] = "LESSEQUALOP",
    [GREATEROP] = "GREATEROP",
    [GREATEREQUALOP] = "GREATEREQUALOP",
    [EQUALOP] = "EQUALOP",
    [NOTEQUALOP] = "NOTEQUALOP",
    [SCANEOF] = "SCANEOF",
    [ERROR] = "ERROR"};

const char *Token_GetName(int id)
{
    /* Get the length of the array to validate id can be used to index the array and prevent oob errors.*/
    size_t arr_len = sizeof(tokensMap) / sizeof(tokensMap[0]);
    const char *r_value;
    if (id < arr_len && id >= 0)
    {
        r_value = tokensMap[id];
    }
    else
    {
        r_value = NULL;
    }

    return r_value;
}

