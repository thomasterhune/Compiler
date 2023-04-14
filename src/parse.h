#ifndef parser_h
#define parser_h
/*!
    \file parse.h
    \brief Parser struct, 'methods' declarations including Parse functions.

    Parse is responsible for validating the syntax of an input file. It reads tokens provided by the scanner and validates that their sequence conforms with the rules of the language.

    It also calls action routines which generate C code in the temp and output files provided on loud. 

    Parse_SystemGoal is the entry point, which should be called only after input and output files are loaded into the scanner and parser. It calls for function corresponding to each unique LHS of a production rule.

    If a lexical error is encountered (invalid character), the character is skipped and an error is printed to the listing file. Parsing will continue with the next available character.

    If a syntax error is encountered within a statement, tokens will be skipped until a semicolon or other end-of-statement symbol is found and information about that error will be printed as a trace in the console and the files.

    \authors Karl Miller, Tom Terhune, Anthony Stepich
    \date March 2023

*/
#include <stdio.h>
#include "generator.h"

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
    /*! The temp file */
    FILE *temp;
    /*! A pointer to the start of the temp file.*/
    fpos_t tempstart;
    /*! A buffer, for printing completed statements. */
    char * buffer;
    /*! The current capacity of the buffer */
    int capacity;
    /*! The length of relevant characters in the buffer, and the write index. */
    int l_buffer;
    /*! A tally of total syntax errors. */
    int errorCount;
    /*! A tally of how many functions have failed, for printing them nestedly. */
    int trace;
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
    \param temp The temp file
*/
void Parser_Load(FILE *out, FILE *list, FILE *temp);

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
Parser buffer
-------------------
*/
#pragma region buffer

/*!
    Doubles the size of the buffer, preserving data in the buffer (But possibly moving it to a different location). parser.buffer will point to a buffer twice as large and parser.capacity will be doubled. If necessary, an old buffer may have been freed. Do not use parser's buffer directly as it may be freed.
*/
void Parser_expandBuffer();

/*! 
    If parser.buffer has been allocated, it is freed. parser.buffer is given memory on the heap equal to the const PARSER_BUFFER_INITIAL_CAPACITY. 

    Also resets parser.l_buffer to 0 and parser.capacity. 

*/
void Parser_clearBuffer();

/*!
    Pushes a char * of length l_word to parser's buffer. Moves parser.l_buffer as necessary. Reallocates the buffer if needed by calling Parser_expandBuffer. Puts a null terminator after the end of valid characters.

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
-------------------
Parse errors
-------------------
*/
#pragma region parse_errors

/*!
    Prints a parse error for when a match failed.
    \param expected_token The token that failed to match.
*/
void ParseError_MatchFailed(int expected_token);

/*!
    Prints a parse error when a next-token lookahead failed. Prints it indented as much as the current trace
    \param actual_token The actual token that was found.
    \param n_expected The number of possible expected tokens.
    \param ... The valid expected tokens.
*/
void ParseError_NextTokenFailed(int actual_token, int n_expected, ...);

/*!
    Prints a parse error for a function failing to the output file and the console. The printing will be indented, with the deepest function left-aligned. This allows tracing of the parse functions that failed. 

    Increments parser.trace by 1 to allow visual indentation.
*/
void ParseError_FunctionFailed(const char * functionName);

/*!
    Attempt ParseError recovery. 

    Called when a statement has a syntax error. Skips over tokens until it reaches a statement-end token, such as ENDIF, ENDWHILE, or SEMICOLON, depending on the parse situation.

    For example, if parsing fails inside a WHILE statement, everything until the next ENDWHILE will be skipped to attempt to recover from the error. 

    Encountering END or SCANEOF will also terminate the skipping feature. 

    Skipping lines is noted in the listing file and the number of tokens and lines skipped is printed to the out file and console. 
    
    This allows some limited recovery from parse errors.

    \param endtoken The endtoken to skip to.
    \returns 0 if it was able to skip to the target endtoken, 1 if it encountered END or SCANEOF before then.

*/
short ParseError_SkipToStatementEnd(int endtoken);

/*!
    Prints a summary of the parse errors to the listing file and the console. 
*/
void Parser_PrintErrorSummary();

/*!
    Returns the error count of parser.
    \returns The number of errors parser had. 
*/
int Parser_GetParseErrCount();

#pragma endregion parse_errors
/*
-------------------------------
Production rule parse functions
-------------------------------
*/
#pragma region production_rule_parse_functions

/*! 
    Called by SystemGoal. Parses the program, then matches a SCANEOF token. 

    Calls the action function start.

    Production 1: <program> -> #start BEGIN <statement list> END
*/
short Parse_Program();

/*! 
    Called by Program. Parses a statement, then possibly processes an additional statement list. 

    Production 2: <statement list> -> <statement> {<statement list>}
*/
short Parse_StatementList();

/*! 
    Called by Program, parses a list of statements

    Production 3: <statement> -> ID := <expression> #assign;
    Production 4: <statement> -> READ ( <id list> );
    Production 5: <statement> -> WRITE ( <expr list> );
    Production 6: <statement> -> IF ( #if <condition> )THEN <StatementList> <IFTail>
    Production 9: <statement> -> WHILE ( #while <condition>)  {<StatementList>} ENDWHILE #closebrace

*/
short Parse_Statement();

/*!
    Parses the end of an IF statement, which may be an ELSE or an ENDIF.

    Production 7: <IFTail> -> ELSE #else <StatementList> ENDIF #closebrace
    Production 8: <IFTail> -> ENDIF #closebrace
*/
short Parse_IfTail();

/*!
    Parses an ID list, which is 1 or more IDs. It's used with the READ production of Statement.

    Production 10: <id list> -> ID #read_id {,<id list> #read_id}
*/
short Parse_IDList();

/*!
    Parses an expression list, which is 1 or more expressions. It's used with the WRITE production of Statement.

    Production 11: <expr list> -> <expression> {, <expr list> #write_expr}
*/
short Parse_ExpressionList();

/*!
    Parses an expression, which begins the parse for arithmetic sequences with order-of-operations.

    Populates the EXPR_RECORD parameter.

    Production 12: <expression> -> <term> {<add op> <term> #gen_infix}
*/
short Parse_Expression(EXPR_RECORD * expr_rec);

/*!
    Continues the inner expression parse by looking for multiplication symbols.

    Production 13: <term> -> <factor> {<mult op> <factor> #gen_infix}

    \param expr_rec Expression record struct
*/
short Parse_Term(EXPR_RECORD * expr_rec);

/*!
    Processes a factor into a parenthesized expression, negative factor, id, or intliteral.

    Production 14: <factor> -> ( <expression> )

    Production 15: <factor> -> - <factor>

    Production 16: <factor> -> <ident>
    
    Production 17: <factor> -> INTLITERAL #process_literal

    \param expr_rec Expression record struct
*/
short Parse_Factor(EXPR_RECORD * expr_rec);

/*!
    Processes the add op, which can be + or -, because they share the same precedence.

    Production 18: <add op> -> + #process_op
    Production 19: <add op> -> - #process_op

    \param expr_rec Expression record struct
*/
short Parse_AddOP();

/*!
    Processes the add op, which can be * or /, because they share the same precedence.

    Production 20: <mult op> -> * #process_op
    Production 21: <mult op> -> / #process_op
*/
short Parse_MultOP(OP_RECORD * op_record);

/*!
    Begins parsing a condition operation.

    Production 22: <condition> -> <addition> {<rel op> <addition> #gen_infix}

    \param op_record Op Record struct
*/
short Parse_Condition(EXPR_RECORD * expr_record);

/*!
    Each side of a logical operation may have arithmetic operations, and precedence must be maintained.

    Production 23: <addition> -> <multiplication> {<add op> <multiplication> #gen_infix}

    \param expr_rec Expression record struct
*/
short Parse_Addition(EXPR_RECORD * expr_record);

/*!
    Each side of a logical operation may have arithmetic operations, and precedence must be maintained.

    Production 24: <multiplication> -> <unary> { <mult op> <unary> #gen_infix}

    \param expr_rec Expression record struct
*/
short Parse_Multiplication(EXPR_RECORD * expr_record);

/*!
    Unary operations may NOT or NEGATE a logical outcome. 

    Production 25: <unary> -> ! <unary>
    Production 26: <unary> -> - <unary>
    Production 27: <unary> -> <lprimary>

    \param expr_rec Expression record struct
*/
short Parse_Unary(EXPR_RECORD * expr_record);

/*!
    LPrimary allows nesting of further conditions or final condition values, such as false and true.

    Produciton 28:  <lprimary> -> INTLITERAL #process_literal
    Produciton 29:  <lprimary> -> ID
    Produciton 30:  <lprimary  -> ( <condition>)
    Produciton 31:  <lprimary> -> FALSEOP #process_op
    Produciton 32:  <lprimary> -> TRUEOP #process_op
    Produciton 33:  <lprimary> -> NULLOP #process_op

    \param expr_rec Expression record struct
*/
short Parse_LPrimary(EXPR_RECORD * expr_record);

/*!
    Relop results in the standard logical operators.

    Produciton 34: <RelOp> -> < #process_op
    Produciton 35: <RelOp> -> <= #process_op
    Produciton 36: <RelOp> -> > #process_op
    Produciton 37: <RelOp> -> >= #process_op
    Produciton 38: <RelOP> -> = #process_op
    Produciton 39: <RelOp> -> <> #process_op

    \param expr_rec Expression record struct
*/
short Parse_RelOP(OP_RECORD * op_record);

/*! 
    Called by main. Begins the parsing process.

    Production 40. <system goal> -> <program> SCANEOF #finish

    \param op_record Op Record struct
*/
short Parse_SystemGoal();

/*!
    Consumes an identifier; action routine will create symbol table entry.
    
    Production 41. <ident> -> ID #process_id
    
    \param expr_record expression record 
*/
short Parse_Ident(EXPR_RECORD *expr_record);

#pragma endregion production_rule_parse_functions

#pragma action_functions
/*!
    initialization of intermediate c code file, symbol table, temp counter, line counter

*/
void Parse_ActionStart();

/*!
    Write descriptive closing to the listing and output files, catenate the c files together

*/
void Parse_ActionFinish();

/*!
    will call generate passing the two contents of the expression records along with the ' = '
    so that a correct C assigment is created

    \param target expression record struct
    \param source expression record struct
    
*/
void Parse_ActionAssign(EXPR_RECORD * target, EXPR_RECORD * source);

/*!
    receive an expression record and generate a printf statement for the read statement

    \param target expression record

*/
void Parse_ActionWriteExpr(EXPR_RECORD * target);

/*!

      Will return a new OP_RECORD.

      It will utilize the 3 character buffer in the op_record for the string. 

      \returns op_rec op record struct
*/
OP_RECORD Parse_ActionProcessOp(int token);

/*!
       process_id
       generates the code for the ID semantic record
       it creates an expression record and sets its kind to IDEXPR
       it sets its string to the contents of the token buffer which is the ID
       \returns expression record struct
*/
EXPR_RECORD Parse_ActionProcessID();

/*! 
    Uses the scanner buffer to generate an expression record for an intliteral.

    \returns An EXPR_RECORD of an intliteral.
*/
EXPR_RECORD Parse_ActionProcessLiteral();

/*!
      - #gen_infix
      - generates the code for the infix semantic record
      - it will accept two expression records for the left hand side and one for the right hand side and an operation record
      - it creates third expression record and sets its kind to TEMPEXPR
      - it sets its string to a new TempID by using gettemp
      - it generates C code with generate for the assignment of the left expression right expression to the Temp
      - i.e. Temp5 = X + 7;
      - \returns it then returns the temp expression record
      - this process will always have pairs of params condensing down to one new temporary which build a more complex expression
      \param left_side expression record struct
      \param op_record  op record struct
      \param right_side expression record struct
      
*/
EXPR_RECORD Parse_ActionGenInfix(EXPR_RECORD * left_side, OP_RECORD *op_record, EXPR_RECORD *right_side);

/*!
    generates scanf statement

    \param target expression record struct
*/
void Parse_ActionReadID(EXPR_RECORD * target);

/*!
    Returns an expression record with prepend prepended to the reference name.
    \param source The expression record to prepend.
    \param prepend The string to prepend.
    \returns An expression record of the same type, with the 'reference' field set to prepend+source.
*/
EXPR_RECORD Parse_PrependedCopy(EXPR_RECORD * source, char * prepend);

/*!
    Returns an expression record with the reference value set as the c-style value of the token. For example, 'NULL' will become '0'.
    \param token the parameter
    \returns a new expr record with a malloced reference containing the c-style token string
*/
EXPR_RECORD Parse_ActionProcessTokenAlias(int token);

/*!
    prints 'if (expression) {' to the temp file

    \param conditional_expression expression record struct
*/
void Parse_ActionIf(EXPR_RECORD * conditional_expression);

/*!
    prints 'while (expression) {' to the temp file
    
    \param conditional_expression expression record struct
*/
void Parse_ActionWhile(EXPR_RECORD * conditional_expression);

/*!
    Prints '}' to the temp file

    \param conditional_expression expression record struct
*/
void Parse_ActionCloseBrace();

/*! 
    Prints '} else {' to the temp file.

    \param conditional_expression expression record struct
*/
void Parse_ActionElse();

#pragma endregion action_functions
#endif