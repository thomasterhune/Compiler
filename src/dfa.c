/*!
    \file dfa.c
    \brief The DFA and related logic definitions.

    The DFA is a 3 dimensional array that maps a given state and character input to a result consisting of the next state, token, and whether reading should continue.

    The DFA was created in Excel, and the excel file is available in docs/fullDFA.xlsx.



    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date February 2023

*/
/*
    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 
*/

#include "tokens.h"
#include "dfa.h"
#include <string.h>
#include <stdio.h>

#pragma region dfaStates
enum DFA_STATES { /* Enumerated values for the DFA states, to aid readability. */
STATE_START, 
STATE_ID,
STATE_ERROR,
STATE_B,
STATE_BE,STATE_BEG,
STATE_BEGI, 
STATE_BEGIN,
STATE_E,
STATE_EN,
STATE_END,
STATE_R,
STATE_RE,
STATE_REA,
STATE_READ,
STATE_I,
STATE_IF,
STATE_T,
STATE_TH,
STATE_THE,
STATE_THEN,
STATE_EL,
STATE_ELS,
STATE_ELSE,
STATE_ENDI,
STATE_ENDIF,
STATE_ENDW,
STATE_ENDWH,
STATE_ENDWHI,
STATE_ENDWHIL,
STATE_ENDWHILE,
STATE_W,
STATE_WH,
STATE_WHI,
STATE_WHIL,
STATE_WHILE,
STATE_F,
STATE_FA,
STATE_FAL,
STATE_FALS,
STATE_FALSE ,
STATE_TR,
STATE_TRU,
STATE_TRUE,
STATE_N,
STATE_NU,
STATE_NUL,
STATE_NULL,
STATE_LPAR,
STATE_RPAR,
STATE_SEMIC,
STATE_COMMA,
STATE_COLON,
STATE_COLONEQUALS,
STATE_PLUS,
STATE_MINUS,
STATE_MULTIPLY,
STATE_DIV,
STATE_NOT,
STATE_LESS,
STATE_LESSEQ,
STATE_GREAT,
STATE_GREATEQ,
STATE_EQ,
STATE_NOTEQ,
STATE_INT,
STATE_EOF,
STATE_WR,
STATE_WRI,
STATE_WRIT,
STATE_WRITE
};

char * GetStateString(int n) {
    char * statename = malloc(20);
    switch(n) {
        case STATE_START:
            strcpy(statename, "STATE_START");
            break;
        case STATE_ID:
            strcpy(statename, "STATE_ID");
            break;
        case STATE_ERROR:
            strcpy(statename, "STATE_ERROR");
            break;
        case STATE_B:
            strcpy(statename, "STATE_B");
            break;
        case STATE_BE:
            strcpy(statename, "STATE_BE");
            break;
        case STATE_BEG:
            strcpy(statename, "STATE_BEG");
            break;
        case STATE_BEGI:
            strcpy(statename, "STATE_BEGI");
            break;
        case STATE_BEGIN:
            strcpy(statename, "STATE_BEGIN");
            break;
        case STATE_E:
            strcpy(statename, "STATE_E");
            break;
        case STATE_EN:
            strcpy(statename, "STATE_EN");
            break;
        case STATE_END:
            strcpy(statename, "STATE_END");
            break;
        case STATE_R:
            strcpy(statename, "STATE_R");
            break;
        case STATE_RE:
            strcpy(statename, "STATE_RE");
            break;
        case STATE_REA:
            strcpy(statename, "STATE_REA");
            break;
        case STATE_READ:
            strcpy(statename, "STATE_READ");
            break;
        case STATE_I:
            strcpy(statename, "STATE_I");
            break;
        case STATE_IF:
            strcpy(statename, "STATE_IF");
            break;
        case STATE_T:
            strcpy(statename, "STATE_T");
            break;
        case STATE_TH:
            strcpy(statename, "STATE_TH");
            break;
        case STATE_THE:
            strcpy(statename, "STATE_THE");
            break;
        case STATE_THEN:
            strcpy(statename, "STATE_THEN");
            break;
        case STATE_EL:
            strcpy(statename, "STATE_EL");
            break;
        case STATE_ELS:
            strcpy(statename, "STATE_ELS");
            break;
        case STATE_ELSE:
            strcpy(statename, "STATE_ELSE");
            break;
        case STATE_ENDI:
            strcpy(statename, "STATE_ENDI");
            break;
        case STATE_ENDIF:
            strcpy(statename, "STATE_ENDIF");
            break;
        case STATE_ENDW:
            strcpy(statename, "STATE_ENDW");
            break;
        case STATE_ENDWH:
            strcpy(statename, "STATE_ENDWH");
            break;
        case STATE_ENDWHI:
            strcpy(statename, "STATE_ENDWHI");
            break;
        case STATE_ENDWHIL:
            strcpy(statename, "STATE_ENDWHIL");
            break;
        case STATE_ENDWHILE:
            strcpy(statename, "STATE_ENDWHILE");
            break;
        case STATE_W:
            strcpy(statename, "STATE_W");
            break;
        case STATE_WH:
            strcpy(statename, "STATE_WH");
            break;
        case STATE_WHI:
            strcpy(statename, "STATE_WHI");
            break;
        case STATE_WHIL:
            strcpy(statename, "STATE_WHIL");
            break;
        case STATE_WHILE:
            strcpy(statename, "STATE_WHILE");
            break;
        case STATE_F:
            strcpy(statename, "STATE_F");
            break;
        case STATE_FA:
            strcpy(statename, "STATE_FA");
            break;
        case STATE_FAL:
            strcpy(statename, "STATE_FAL");
            break;
        case STATE_FALS:
            strcpy(statename, "STATE_FALS");
            break;
        case STATE_FALSE:
            strcpy(statename, "STATE_FALSE");
            break;
        case STATE_TR:
            strcpy(statename, "STATE_TR");
            break;
        case STATE_TRU:
            strcpy(statename, "STATE_TRU");
            break;
        case STATE_TRUE:
            strcpy(statename, "STATE_TRUE");
            break;
        case STATE_N:
            strcpy(statename, "STATE_N");
            break;
        case STATE_NU:
            strcpy(statename, "STATE_NU");
            break;
        case STATE_NUL:
            strcpy(statename, "STATE_NUL");
            break;
        case STATE_NULL:
            strcpy(statename, "STATE_NULL");
            break;
        case STATE_LPAR:
            strcpy(statename, "STATE_LPAR");
            break;
        case STATE_RPAR:
            strcpy(statename, "STATE_RPAR");
            break;
        case STATE_SEMIC:
            strcpy(statename, "STATE_SEMIC");
            break;
        case STATE_COMMA:
            strcpy(statename, "STATE_COMMA");
            break;
        case STATE_COLON:
            strcpy(statename, "STATE_COLON");
            break;
        case STATE_COLONEQUALS:
            strcpy(statename, "STATE_COLONEQUALS");
            break;
        case STATE_PLUS:
            strcpy(statename, "STATE_PLUS");
            break;
        case STATE_MINUS:
            strcpy(statename, "STATE_MINUS");
            break;
        case STATE_MULTIPLY:
            strcpy(statename, "STATE_MULTIPLY");
            break;
        case STATE_DIV:
            strcpy(statename, "STATE_DIV");
            break;
        case STATE_NOT:
            strcpy(statename, "STATE_NOT");
            break;
        case STATE_LESS:
            strcpy(statename, "STATE_LESS");
            break;
        case STATE_LESSEQ:
            strcpy(statename, "STATE_LESSEQ");
            break;
        case STATE_GREAT:
            strcpy(statename, "STATE_GREAT");
            break;
        case STATE_GREATEQ:
            strcpy(statename, "STATE_GREATEQ");
            break;
        case STATE_EQ:
            strcpy(statename, "STATE_EQ");
            break;
        case STATE_NOTEQ:
            strcpy(statename, "STATE_NOTEQ");
            break;
        case STATE_INT:
            strcpy(statename, "STATE_INT");
            break;
        case STATE_EOF:
            strcpy(statename, "STATE_EO");
            break;
        case STATE_WR:
            strcpy(statename, "STATE_WR");
            break;
        case STATE_WRI:
            strcpy(statename, "STATE_WRI");
            break;
        case STATE_WRIT:
            strcpy(statename, "STATE_WRIT");
            break;
        case STATE_WRITE:
            strcpy(statename, "STATE_WRITE");
            break;
        default:
            strcpy(statename, "????");
            break;
    }
    return statename;
    
}

#pragma endregion dfaStates

#pragma region dfa

/*! 

    The DFA drives the scanner logic. Each of the 71 rows in this state transition table corresponds to a state, or a node on a DFA graph. 

    Each column corresponds to an edge, with columns 0-25 being 'a' through 'z'. There are also operator characters. 
    The ASCII code of a character is not it's column position and characters must be converted to column numbers before they can
    index this state transition table. 
    At each cell, there are three values. First is the next state to transition to. Second is the token. Third is
    a signal to the DFA driver whether to continue reading or not. It says, 'this character is a boundary character for this state'.

    The DFA was generated in Excel, and that .xlsx file is available in the /docs folder of this project. 
*/


short DFA[71][44][3] = {
{{STATE_ID, ID, 1},{STATE_B, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_E, ID, 1},{STATE_F, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_I, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_N, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_R, ID, 1},{STATE_ID, ID, 1},{STATE_T, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_W, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_START, LEX_ERROR, 0},{STATE_LPAR,  LPAREN, 1},{STATE_RPAR, RPAREN, 1},{STATE_SEMIC, SEMICOLON, 1},{STATE_COMMA, COMMA, 1},{STATE_COLON, LEX_ERROR, 1},{STATE_EQ, EQUALOP, 1},{STATE_PLUS, PLUSOP, 1},{STATE_MINUS, MINUSOP, 1},{STATE_MULTIPLY, MULTOP, 1},{STATE_DIV, DIVOP, 1},{STATE_NOT, NOTOP, 1},{STATE_LESS, LESSOP, 1},{STATE_GREAT, GREATEROP, 1},{STATE_INT, INTLITERAL, 1},{STATE_EOF, SCANEOF, 1},{STATE_ERROR, LEX_ERROR, 1},{STATE_ERROR, LEX_ERROR, 1}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_BE, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_BEG, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_BEGI, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_BEGIN, BEGIN, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_ID, ID, 1},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0},{STATE_BEGIN, BEGIN, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_EL, ID, 1},{STATE_ID, ID, 1},{STATE_EN, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_END, END, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDI, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDW, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_ID, ID, 1},{STATE_END, END, 0},{STATE_END, END, 0},{STATE_END, END, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_RE, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_REA, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_READ, READ, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_ID, ID, 1},{STATE_READ, READ, 0},{STATE_READ, READ, 0},{STATE_READ, READ, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_IF, IF, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_ID, ID, 1},{STATE_IF, IF, 0},{STATE_IF, IF, 0},{STATE_IF, IF, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_TH, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_TR, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_THE, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_THEN, THEN, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_ID, ID, 1},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0},{STATE_THEN, THEN, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ELS, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ELSE, ELSE, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ID, ID, 1},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDIF, ENDIF, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ID, ID, 1},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0},{STATE_ELSE, ELSE, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ID, ID, 1},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0},{STATE_ENDIF, ENDIF, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDWH, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDWHI, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDWHIL, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDWHILE, ENDWHILE, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ID, ID, 1},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0},{STATE_ENDWHILE, ENDWHILE, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WH, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WR, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WHI, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WHIL, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WHILE, WHILE, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_ID, ID, 1},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0},{STATE_WHILE, WHILE, 0}},
{{STATE_FA, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_FAL, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_FALS, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_FALSE, FALSEOP, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_ID, ID, 1},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0},{STATE_FALSE, FALSEOP, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_TRU, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_TRUE, TRUEOP, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_ID, ID, 1},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0},{STATE_TRUE, TRUEOP, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_NU, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_NUL, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_NULL, NULLOP, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_ID, ID, 1},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0},{STATE_NULL, NULLOP, 0}},
{{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0},{STATE_LPAR, LPAREN, 0}},
{{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0},{STATE_RPAR, RPAREN, 0}},
{{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0},{STATE_SEMIC, SEMICOLON, 0}},
{{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0},{STATE_COMMA, COMMA, 0}},
{{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_COLONEQUALS, ASSIGNOP, 1},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0}},
{{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_ID, ID, 1},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0},{STATE_COLONEQUALS, ASSIGNOP, 0}},
{{STATE_PLUS, PLUSOP, 0},{STATE_ID, ID, 1},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_INT, INTLITERAL, 1},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0},{STATE_PLUS, PLUSOP, 0}},
{{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_INT, INTLITERAL, 1},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0},{STATE_MINUS, MINUSOP, 0}},
{{STATE_MULTIPLY, MULTOP, 0},{STATE_ID, ID, 1},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0},{STATE_MULTIPLY, MULTOP, 0}},
{{STATE_DIV, DIVOP, 0},{STATE_ID, ID, 1},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0},{STATE_DIV, DIVOP, 0}},
{{STATE_NOT, NOTOP, 0},{STATE_ID, ID, 1},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0},{STATE_NOT, NOTOP, 0}},
{{STATE_LESS, LESSOP, 0},{STATE_ID, ID, 1},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESSEQ, LESSEQUALOP, 1},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_NOTEQ, NOTEQUALOP, 1},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0},{STATE_LESS, LESSOP, 0}},
{{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_ID, ID, 1},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0},{STATE_LESSEQ, LESSEQUALOP, 0}},
{{STATE_GREAT, GREATEROP, 0},{STATE_ID, ID, 1},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREATEQ, GREATEREQUALOP, 1},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0},{STATE_GREAT, GREATEROP, 0}},
{{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_ID, ID, 1},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0},{STATE_GREATEQ, GREATEREQUALOP, 0}},
{{STATE_EQ, EQUALOP, 0},{STATE_ID, ID, 1},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0},{STATE_EQ, EQUALOP, 0}},
{{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_ID, ID, 1},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0},{STATE_NOTEQ, NOTEQUALOP, 0}},
{{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 1},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0},{STATE_INT, INTLITERAL, 0}},
{{STATE_ERROR, LEX_ERROR, 0},{STATE_ID, ID, 1},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0},{STATE_ERROR, LEX_ERROR, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WRI, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WRIT, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_WRITE, WRITE, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},
{{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 1},{STATE_ID, ID, 0},{STATE_ID, ID, 0},{STATE_ID, ID, 0}},

}
;


#pragma endregion dfa



#pragma region dfa_cols

/* The characters correspond to the DFA columns.*/
enum DFA_CHARS{
    CH_A,CH_B,CH_C,CH_D,CH_E,CH_F,CH_G,CH_H,CH_I,CH_J,CH_K,CH_L,CH_M,CH_N,CH_O,CH_P,CH_Q,CH_R,CH_S,CH_T,CH_U,CH_V,CH_W,CH_X,CH_Y,CH_Z,CH_WSPC,CH_LPRN,CH_RPRN,CH_SEMIC,CH_COMM,CH_COLON,CH_EQU,CH_PLUS,CH_MINUS,CH_STAR,CH_DIV,CH_NOT,CH_LT,CH_GT,CH_NUM,CH_EOF,CH_NOTINSET, CH_NLINE
};

/*! 
GetDFAColString returns the column name associated with a given number. It is used only for debugging the DFA. 
*/
char * GetDFAColString(int n) {
    char * str = malloc(15);
    switch(n) {
        case CH_A:
            strcpy(str, "CH_A");
            break;
    case CH_B:
        strcpy(str, "CH_B");
        break;
    case CH_C:
        strcpy(str, "CH_C");
        break;
    case CH_D:
        strcpy(str, "CH_D");
        break;
    case CH_E:
        strcpy(str, "CH_E");
        break;
    case CH_F:
        strcpy(str, "CH_F");
        break;
    case CH_G:
        strcpy(str, "CH_G");
        break;
    case CH_H:
        strcpy(str, "CH_H");
        break;
    case CH_I:
        strcpy(str, "CH_I");
        break;
    case CH_J:
        strcpy(str, "CH_J");
        break;
    case CH_K:
        strcpy(str, "CH_K");
        break;
    case CH_L:
        strcpy(str, "CH_L");
        break;
    case CH_M:
        strcpy(str, "CH_M");
        break;
    case CH_N:
        strcpy(str, "CH_N");
        break;
    case CH_O:
        strcpy(str, "CH_O");
        break;
    case CH_P:
        strcpy(str, "CH_P");
        break;
    case CH_Q:
        strcpy(str, "CH_Q");
        break;
    case CH_R:
        strcpy(str, "CH_R");
        break;
    case CH_S:
        strcpy(str, "CH_S");
        break;
    case CH_T:
        strcpy(str, "CH_T");
        break;
    case CH_U:
        strcpy(str, "CH_U");
        break;
    case CH_V:
        strcpy(str, "CH_V");
        break;
    case CH_W:
        strcpy(str, "CH_W");
        break;
    case CH_X:
        strcpy(str, "CH_X");
        break;
    case CH_Y:
        strcpy(str, "CH_Y");
        break;
    case CH_Z:
        strcpy(str, "CH_Z");
        break;
    case CH_WSPC:
        strcpy(str, "CH_WSPC");
        break;
    case CH_LPRN:
        strcpy(str, "CH_LPRN");
        break;
    case CH_RPRN:
        strcpy(str, "CH_RPRN");
        break;
    case CH_SEMIC:
        strcpy(str, "CH_SEMIC");
        break;
    case CH_COMM:
        strcpy(str, "CH_COMM");
        break;
    case CH_COLON:
        strcpy(str, "CH_COLON");
        break;
    case CH_EQU:
        strcpy(str, "CH_EQU");
        break;
    case CH_PLUS:
        strcpy(str, "CH_PLUS");
        break;
    case CH_MINUS:
        strcpy(str, "CH_MINUS");
        break;
    case CH_STAR:
        strcpy(str, "CH_STAR");
        break;
    case CH_DIV:
        strcpy(str, "CH_DIV");
        break;
    case CH_NOT:
        strcpy(str, "CH_NOT");
        break;
    case CH_LT:
        strcpy(str, "CH_LT");
        break;
    case CH_GT:
        strcpy(str, "CH_GT");
        break;
    case CH_NUM:
        strcpy(str, "CH_NUM");
        break;
    case CH_EOF:
        strcpy(str, "CH_EOF");
        break;
    case CH_NOTINSET:
        strcpy(str, "CH_NOTINSET");
        break;
    case CH_NLINE:
        strcpy(str, "CH_NLINE");
        break;
    default:
        strcpy(str, "????");
    }
    return str;
}

short GetDFAColumn(char c) {
    short column_code;
    if(c >= '0' && c <= '9') {
        column_code = CH_NUM; 
    } else if (c >= 'A' && c <='Z') {
        column_code = c - 'A'; /* alphabets are from 0 to 25... eg 0 = CH_A, 1 = CH_B and so on.*/
    } else if(c >= 'a' && c <= 'z') {
        column_code = c - 'a';
    } else {
        switch(c) {
            case '\n':
                column_code = CH_NLINE;
                break;
            case '\t':
                column_code = CH_WSPC;
                break;
            case ' ':
                column_code = CH_WSPC;
                break;
            case '(':
                column_code = CH_LPRN;
                break;
            case ')':
                column_code = CH_RPRN;
                break;
            case ';':
                column_code = CH_SEMIC;
                break;
            case ',':
                column_code = CH_COMM;
                break;
            case ':':
                column_code = CH_COLON;
                break;
            case '=':
                column_code = CH_EQU;
                break;
            case '+':
                column_code = CH_PLUS;
                break;
            case '-':
                column_code = CH_MINUS;
                break;
            case '*':
                column_code = CH_STAR;
                break;
            case '/':
                column_code = CH_DIV;
                break;
            case '!':
                column_code = CH_NOT;
                break;
            case '<':
                column_code = CH_LT;
                break;
            case '>':
                column_code = CH_GT;
                break;
            case -1:
                column_code = CH_EOF;
                break;
            default:
                column_code = CH_NOTINSET;
                break;
        }
    }
    return column_code;
}

#pragma endregion dfa_cols


#pragma region dfa_driver


int GetNextToken(FILE * file, int * charsRead) {
    *charsRead = 0;
    short token = LEX_ERROR;
    short state = STATE_START;
    short keepReading = 1;
    char lastchar = getc(file);
    short read = GetDFAColumn(lastchar);
    while(read == CH_WSPC)
    { /* skip leading whitespace. */
        lastchar = getc(file);
        read = GetDFAColumn(lastchar);
    }
    /* printf("\n-- ws skip done, starting parse -- "); */
    /* printStateAndChar(state, read); */
    while(keepReading) {
        token = DFA[state][read][1];
        /* printf("\n\tToken At state: %s", Token_GetName(token)); */
        state = DFA[state][read][0];
        lastchar = getc(file);
        read = GetDFAColumn(lastchar);
        *charsRead += 1;
        keepReading = DFA[state][read][2];
        /* printf("\n\tKeep reading? %d", keepReading);
        printStateAndChar(state, read); */
    }
    ungetc(lastchar, file);
    /* printf("\n>> RETURNING %s", Token_GetName(token)); */
    return token;
}


int GetNextTokenInBuffer(char * buffer, int * bufIndex, int * charsRead) {
    *charsRead = 0;
    short token = LEX_ERROR;
    short state = STATE_START;
    short keepReading = 1;
    short read = GetDFAColumn(buffer[*bufIndex]);
    while(read == CH_WSPC)
    { /* skip leading whitespace. */
        *bufIndex += 1;
        read = GetDFAColumn(buffer[*bufIndex]);
    }
    /* printf("\n-- ws skip done, starting parse -- ");
       printStateAndChar(state, read); */
    while(keepReading) {
        token = DFA[state][read][1];
        /* printf("\n\tToken At state: %s", Token_GetName(token)); */
        state = DFA[state][read][0];
        *bufIndex += 1;
        *charsRead += 1;
        read = GetDFAColumn(buffer[*bufIndex]);
        keepReading = DFA[state][read][2];
        /* printf("\n\tKeep reading? %d", keepReading); 
           printStateAndChar(state, read);  */
    }
    return token;

}

void printCell(int row, int col) {
    printf("\n[%d,%d,%d]", DFA[row][col][0], DFA[row][col][1], DFA[row][col][2]);
}

void printStateAndChar(int row, int col) {
    char * s1 = GetDFAColString(col);
    char * s2 = GetStateString(row);
    printf("\n%s : %s", s2, s1);
    free(s1);
    free(s2);
}

#pragma endregion dfa_driver