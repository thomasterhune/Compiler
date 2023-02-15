#include "tokens.h"
#include <string.h>

#include <stdio.h>

/*!
    \file tokens.c
    \brief Token map and related functions

    The tokensMap maps a given token to a constant string, which is used by Token_GetName() to get the name of a token. The index of a token string in the tokensMap is the same as it's enumerated value. E.G, BEGIN is value 0 and "BEGIN" is at position 0 in the tokensMap array. 

    This file also contains definitions for TokenCatch methods, which are no longer used. In an earlier version of the program, a TokenCatch wrapped a given token with related data and was memory-allocated. The current version does not use TokenCatch, but it is retained here in case we need it for future parsing features. 

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/

/*

    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 

*/

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

/*!
------------------------------------------------------------------
    Note: TokenCatch is no longer used. It was used in an earlier version of this program. It may be revived in the future depending on the needs of the parser. 
------------------------------------------------------------------
*/
#pragma region token_catch
struct TokenCatch* Token_Catch(short tokenType, char* raw_text_found, int line_found_at, int col_found_at) {
    struct TokenCatch* tc = malloc(sizeof (struct TokenCatch));
    tc->token = tokenType;
    tc->raw = raw_text_found;
    tc->line_no = line_found_at;
    tc->col_no = col_found_at;
    return tc;
}

char * Token_GetOpRaw(short tokenType) {
    char * result;
    switch(tokenType) {
        case LPAREN:
            result = "(";
            break;
        case RPAREN:
            result = ")";
            break;
        case SEMICOLON:
            result = ";";
            break;
        case COMMA:
            result = ",";
            break;
        case ASSIGNOP:
            result = ":=";
            break;
        case PLUSOP:
            result = "+";
            break;
        case MINUSOP:
            result = "-";
            break;
        case MULTOP:
            result = "*";
            break;
        case DIVOP:
            result = "/";
            break;
        case NOTOP:
            result = "!";
            break;
        case LESSOP:
            result = "<";
            break;
        case LESSEQUALOP:
            result = "<=";
            break;
        case GREATEROP:
            result = ">";
            break;
        case GREATEREQUALOP:
            result = ">=";
            break;
        case EQUALOP:
            result = "=";
            break;
        case NOTEQUALOP:
            result = "<>";
            break;
        default:
            result = ":";
    }
    char * ret_val = malloc( (strlen(result)+1)*sizeof(char));
    strcpy(ret_val, result);
    return ret_val;
}


struct TokenCatch* Token_CatchOp(short tokenType, int line_found_at, int col_found_at) {
    struct TokenCatch* tc = malloc(sizeof(struct TokenCatch));
    tc->token = tokenType;
    tc->raw = Token_GetOpRaw(tokenType);
    tc->line_no = line_found_at;
    tc->col_no = col_found_at;
    return tc;
}

struct TokenCatch* Token_CatchError(char badChar, int line_found_at, int col_found_at) {
    struct TokenCatch* tc = malloc(sizeof(struct TokenCatch));
    tc->token = ERROR;
    tc->raw = malloc(2*sizeof(char));
    tc->raw[0] = badChar;
    tc->raw[1] = '\0';
    tc->line_no = line_found_at;
    tc->col_no = col_found_at;
}

void Token_Destroy(struct TokenCatch* token) {
    free(token->raw); /* dealloc the string first. */
    free(token);
}
#pragma endregion token_catch
