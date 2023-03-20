#ifndef parser_h
#define parser_h
#include <stdio.h>

#define PARSER_BUFFER_INITIAL_CAPACITY 50
/*
-----------------------------
Typedef for the parser struct
-----------------------------
*/
#pragma region typedefs
typedef struct {
    /*! The output file */
    FILE * out;
    /*! The listing file */
    FILE * list;
    /*! A buffer, for printing completed statements. */
    char * buffer;
    /*! The current capacity of the buffer */
    int capacity;
    /*! The length of relevant characters in the buffer, and the write index. */
    int l_buffer;
    /*! A tally of syntax errors. */
    int errorCount;
} T_Parser;

#pragma endregion typedefs

/*
--------------------------------
Lifecycle methods for the parser
---------------------------------
*/
#pragma region lifecycle
/*!
    Parser_Load loads the output and listing files to the parser struct so additional information can be printed to them.

    It also loads a pointer to the scanners buffer length, so that it can use the scanner's buffer for its own printing operations.

    \param out The output file
    \param list The listing file
*/
void Parser_Load(FILE *out, FILE *list);

/*! 
    Performs any initialization needed by the parser.
*/
void Parser_Init();

/*!
    Deinitializes the parser, setting file references to NULL (but not closing the files.)
*/
void Parser_DeInit();

#pragma endregion lifecycle

/*
-------------------
Scanner buffer
-------------------
*/
#pragma region buffer

/*!
    Doubles the size of the buffer, preserving data in the buffer (But possibly moving it to a different location). parser.buffer will point to a buffer twice as large and parser.capacity will be doubled. If necessary, an old buffer may have been freed. Do not use scanner's buffer directly as it may be freed.
*/
void Parser_expandBuffer();

/*! 
    If parser.buffer has been allocated, it is freed. parser.buffer is given memory on the heap equal to the const PARSER_BUFFER_INITIAL_CAPACITY. 

    Also resets parser.l_buffer to 0 and parser.capacity. 

*/
void Parser_clearBuffer();

/*!
    Pushes a char * of length l_word to parser's buffer. Moves parser.l_buffer as necessary. Reallocates the buffer if needed by calling Parser_expandBuffer. 

    \param word A char array to add to the buffer.
    \param l_word The length of word, in chars. 
*/
void Parser_pushToBuffer(char * word, int l_word);


/*!
    Prints the contents of the buffer to parser.out. Prepends "Statement: " to the printed text. Inserts newlines before and after. "Clears" the buffer afterwards, by setting the length to 0.
*/
void Parser_printBufferStatementToOutAndClear();



#pragma endregion buffer

/*
-------------------------------
Production rule parse functions
-------------------------------
*/
#pragma region production_rule_parse_functions

/*! 
    Called by SystemGoal. Parses the program, then matches a SCANEOF token. 

    Production 1: <program> -> BEGIN <statement list> END
*/
short Parse_Program();

/*! 
    Called by Program. Parses a statement, then possibly processes an additional statement list. 

    Production 2: <statement list> -> <statement> {<statement list>}
*/
short Parse_StatementList();

/*! 
    Called by Program, parses a list of statements

    Production 3: <statement> -> ID := <expression>;
    Production 4: <statement> -> READ ( <id list> );
    Production 5: <statement> -> WRITE ( <expr list> );
    Production 6: <statement> -> IF ( <condition> )THEN <StatementList> <IFTail>
    Production 9: <statement> -> WHILE ( <condition>)  {<StatementList>} ENDWHILE

*/
short Parse_Statement();

/*!
    Parses the end of an IF statement, which may be an ELSE or an ENDIF.

    Production 7: <IFTail> -> ELSE <StatementList> ENDIF
    Production 8: <IFTail> -> ENDIF
*/
short Parse_IfTail();

/*!
    Parses an ID list, which is 1 or more IDs. It's used with the READ production of Statement.

    Production 10: <id list> -> ID {,<id list> }
*/
short Parse_IDList();

/*!
    Parses an expression list, which is 1 or more expressions. It's used with the WRITE production of Statement.

    Production 11: <expr list> -> <expression> {, <expr list>}
*/
short Parse_ExpressionList();

/*!
    Parses an expression, which begins the parse for arithmetic sequences with order-of-operations.

    Production 12: <expression> -> <term> {<add op> <term>}
*/
short Parse_Expression();

/*!
    Continues the inner expression parse by looking for multiplication symbols.

    Production 13: <term> -> <factor> {<mult op> <factor>}
*/
short Parse_Term();

/*!
    Processes a factor into a parenthesized expression, negative factor, id, or intliteral.

    Production 14: <factor> -> ( <expression> )
    Production 15: <factor> -> - <factor>
    Production 16: <factor> -> ID
    Production 17: <factor> -> INTLITERAL
*/
short Parse_Factor();

/*!
    Processes the add op, which can be + or -, because they share the same precedence.

    Production 18: <add op> -> +
    Production 19: <add op> -> -
*/
short Parse_AddOP();

/*!
    Processes the add op, which can be * or /, because they share the same precedence.

    Production 20: <mult op> -> *
    Production 21: <mult op> -> /
*/
short Parse_MultOP();

/*!
    Begins parsing a condition operation.

    Production 22: <condition> -> <addition> {<rel op> <addition>}
*/
short Parse_Condition();

/*
    Each side of a logical operation may have arithmetic operations, and precedence must be maintained.

    Production 23: <addition> -> <multiplication> {<add op> <multiplication>}
*/
short Parse_Addition();

/*
    Each side of a logical operation may have arithmetic operations, and precedence must be maintained.

    Production 24: <multiplication> -> <unary> { <mult op> <unary>}
*/
short Parse_Multiplication();

/*
    Unary operations may NOT or NEGATE a logical outcome. 

    Production 25: <unary> -> ! <unary>
    Production 26: <unary> -> - <unary>
    Production 27: <unary> -> <lprimary>
*/
short Parse_Unary();

/*
    LPrimary allows nesting of further conditions or final condition values, such as false and true.

    Produciton 28:  <lprimary> -> INTLITERAL
    Produciton 29:  <lprimary> -> ID
    Produciton 30:  <lprimary  -> ( <condition>)
    Produciton 31:  <lprimary> -> FALSEOP
    Produciton 32:  <lprimary> -> TRUEOP
    Produciton 33:  <lprimary> -> NULLOP
*/
short Parse_LPrimary();

/*
    Relop results in the standard logical operators.

    Produciton 34: <RelOp> -> <
    Produciton 35: <RelOp> -> <=
    Produciton 36: <RelOp> -> >
    Produciton 37: <RelOp> -> >=
    Produciton 38: <RelOP> -> =
    Produciton 39: <RelOp> -> <>
*/
short Parse_RelOP();

/*! 
    Called by main. Begins the parsing process.

    Production 40. <system goal> -> <program> SCANEOF
*/
short Parse_SystemGoal();

#pragma endregion production_rule_parse_functions

#endif