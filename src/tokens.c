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
int keywordsST[39][26][2] = {{{38, ID},{1, ID},{38, ID},{38, ID},{6, ID},{38, ID},{38, ID},{38, ID},{32, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{19, ID},{38, ID},{34, ID},{38, ID},{38, ID},{23, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{2, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{3, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{4, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{5, BEGIN},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{7, ID},{38, ID},{10, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{8, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{9, ELSE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{11, END},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{12, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{14, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{13, ENDIF},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{15, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{16, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{17, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{18, ENDWHILE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{20, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{21, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{22, READ},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{28, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{24, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{25, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{26, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{27, WRITE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{29,ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{30,ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{31, WHILE},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{33, IF},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{35, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{36, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{37, THEN},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}},{{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID},{38, ID}}};




int Token_RecognizeKeyword(char *word, int length)
{
    int i = 0;
    int state = 0; /* corresponds to the row of the transition table. */
    int token;
    char c;
    while(i < length) { /* Put the word through the transition table / DFA*/
        c = word[i];
        if (c < 'a' || c > 'z') {
            i = length;
        } else {
            c -= 'a'; /* the letter offset corresponds to the column of the transition table. */
            token = keywordsST[state][c][1];
            state = keywordsST[state][c][0];
            i++;
        }
    }
    return token;
}
