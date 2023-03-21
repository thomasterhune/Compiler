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

/*!
------------------------------------------------------------------
    Note: TokenCatch is no longer used. It was used in an earlier version of this program. It may be revived in the future depending on the needs of the parser. 
------------------------------------------------------------------
*/
#pragma region token_catch
struct TokenCatch{
    /* A type corresponding to the TOKEN enum. */
    short token;
    /* The character that was found. */
    char * raw;
    /* The line number it was found on. */
    int line_no;
    /* The column where it started. */
    int col_no;

};

/*!
    Token_Catch is called when an actual token has been found. It produces a TokenCatch struct which wraps the token type with other associated data, such as the raw text that was found and the line it was found at.
    \param tokenType A type in enum TOKEN
    \param raw_text_found A char pointer to the raw text that caused this token to be identified as such.
    \param line_found_at The line in the file the token was found.
    \param col_found_at The column at which the token was found. 
    \returns A new TokenCatch encapsulating the parameter data. 
*/
struct TokenCatch* Token_Catch(short tokenType, char* raw_text_found, int line_found_at, int col_found_at);

/*! Token_GetOpName gets a malloced string for assignment to raw representing what must have been found for an operator text given an enumerated operarator token. If its not one of the operators, it returns ':', which is the one case when a valid operator character was a syntactic error.

    \param tokenType The operator token enumerated id
    \return A malloced string containing the operator, e.g. "<=".
*/
char * Token_GetOpRaw(short tokenType);

/*!
    Token_Catch_Op is called when an op is found. It still produces a TokenCatch but it infers the text that was found based on the token type rather than needing the raw text, since there is not variation in how the operators can be written.  
    \param tokenType A type in enum TOKEN
    \param raw_text_found A char pointer to the raw text that caused this token to be identified as such.
    \param line_found_at The line in the file the token was found.
    \param col_found_at The column at which the token was found. 
    \returns A new TokenCatch encapsulating the parameter data. 
*/
struct TokenCatch* Token_CatchOp(short tokenType, int line_found_at, int col_found_at);

/*!
    Token_CatchError is called when an error is found. Whatever character is passed in will become the 'raw' member of a TokenCatch. 
    \param tokenType A type in enum TOKEN
    \param raw_text_found A char pointer to the raw text that caused this token to be identified as such.
    \param line_found_at The line in the file the token was found.
    \param col_found_at The column at which the token was found. 
    \returns A pointer to a malloced TokenCatch encapsulating the parameter data. 
*/
struct TokenCatch* Token_CatchError(char badChar, int line_found_at, int col_found_at);

/*!
    Token Destroy deallocates a token by first freeing the internal 'raw' string, then deallocating the token itself. 
    \param token A token to deallocate.
*/
void Token_Destroy(struct TokenCatch* token);

#pragma endregion token_catch

#endif