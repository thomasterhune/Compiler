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

/* A transition table / DFA for keywords. */
int keywordsST[52][26][2] = {{{38, ID},{1, ID},{38, ID},{38, ID},{6, ID},{39, ID},{38, ID},{38, ID},{32, ID},{38, ID},{38, ID},{38, ID},{38, ID},{48, ID},{38, ID},{38, ID},{38, ID},{19, ID},{38, ID},{34, ID},{38, ID},{38, ID},{23, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{2, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{3, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{4, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{5, BEGIN},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{7, ID},{38, ID},{10, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{8, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{9, ELSE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{11, END},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{12, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{14, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{13, ENDIF},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{15, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{16, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{17, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{18, ENDWHILE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{20, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{21, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{22, READ},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{28, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{24, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{25, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{26, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{27, WRITE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{29,ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{30,ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{31, WHILE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{33, IF},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{35, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{45, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{36, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{37, THEN},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{40, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{41, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{42, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{43, FALSEOP},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR},{44, ERROR}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{46, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{47, TRUEOP},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{49, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{50, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{51, NULLOP},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}}}

;




int Token_RecognizeKeyword(char *word, int length)
{
    int i = 0;
    int state = 0; /* corresponds to the row of the transition table. */
    int token;
    char c;
    while(i < length) { /* Put the word through the transition table / DFA*/
        c = word[i];
        if (c >= 'A' && c <= 'Z') {
            /* if it's a capital letter, make it lowercase. */
            c = c | 0x60;
        }
        if(c >='0' && c <='9') {
            /* if a number is involved, it's an identifier, because no keywords have numbers. We can already guarantee that it won't start with a number because if it did it would have been already extracted as an int. */
            token = ID;
            state = 38;
        }
        else if (c < 'a' || c > 'z') {
            /* If it's outside the alphabet and wasn't a number, it's a syntax error. The word will always be extracted on operator boundaries, so if a non alphabetic character is found here it isn't defined in our sigma. */
            i = length; /* 'break the loop. */
            token = ERROR;
        } else {
            /* If we reach this branch, we have a character that can cause a transition in our table. */
            c -= 'a'; /* the letter offset corresponds to the column of the transition table. */
            token = keywordsST[state][c][1];
            state = keywordsST[state][c][0];
        }
        i++;
    }
    return token;
}

TokenCatch Token_Catch(short tokenType, char* raw_text_found, int line_found_at, int col_found_at) {
    TokenCatch tc;
    tc.token = tokenType;
    tc.raw = raw_text_found;
    tc.line_no = line_found_at;
    tc.col_no = col_found_at;
    return tc;
}


TokenCatch Token_CatchOp(short tokenType, int line_found_at, int col_found_at) {
    TokenCatch tc;
    tc.token = tokenType;
    const char * tokName;
    if(tc.token == ERROR) {
        /* If tok is error, then it MUST have been ':' */
        tokName = ":";
    } else {
        tokName = Token_GetName(tokenType);
    }
    tc.raw = malloc(strlen(tokName) *sizeof(char));
    strcpy(tc.raw, tokName);
    tc.line_no = line_found_at;
    tc.col_no = col_found_at;
    return tc;
}

TokenCatch Token_CatchError(char badChar, int line_found_at, int col_found_at) {
    TokenCatch tc;
    tc.token = ERROR;
    tc.raw = malloc(2*sizeof(char));
    tc.raw[0] = badChar;
    tc.raw[1] = '\0';
    tc.line_no = line_found_at;
    tc.col_no = col_found_at;
}
