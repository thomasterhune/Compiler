#ifndef tokens_h
#define tokens_h

enum TOKEN {
    BEGIN=0, END, READ, WRITE, IF, THEN, ELSE, ENDIF, WHILE, ENDWHILE, ID, INTLITERAL, FALSEOP, TRUEOP, NULLOP, LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP, MULTOP,DIVOP, NOTOP, LESSOP, LESSEQUALOP, GREATEROP, GREATEREQUALOP, EQUALOP, NOTEQUALOP, SCANEOF, ERROR
};

typedef struct {
    /* A type corresponding to the TOKEN enum. */
    short token;
    /* The character that was found. */
    char * raw;
    /* The line number it was found on. */
    int line_no;
    /* The column where it started. */
    int col_no;

} TokenCatch;

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


/*!
    Token_Catch is called when an actual token has been found. It produces a TokenCatch struct which wraps the token type with other associated data, such as the raw text that was found and the line it was found at.
    \param tokenType A type in enum TOKEN
    \param raw_text_found A char pointer to the raw text that caused this token to be identified as such.
    \param line_found_at The line in the file the token was found.
    \param col_found_at The column at which the token was found. 
    \returns A new TokenCatch encapsulating the parameter data. 
*/
TokenCatch Token_Catch(short tokenType, char* raw_text_found, int line_found_at, int col_found_at);

#endif