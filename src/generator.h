#ifndef generator_h
#define generator_h

#include <stdlib.h>
#include <stdio.h>

#include <tokens.h>

#pragma region defines

#define SYMBOL_TABLE_START_CAPACITY 100

#pragma endregion defines

#pragma region typedefs

/*! OP_RECORD.data is a 3 character array; max two for the operation string plus a null terminator. */
typedef struct {
   char data[3];
} OP_RECORD;

enum EXPR_TYPE {
   EXPR_ID, EXPR_INTLIT, EXPR_TEMP
};

/*!
   EXPR_RECORD holds an expression and a type of expression.
   The enum EXPR_TYPE defines the type of expression.
   The reference portion is an ID, inliteral, or temp ID.
   
*/
typedef struct {
   /*! Can be an ID, intliteral, or Temp ID*/
   char * reference;
   /*! An EXPR_TYPE indicating what type the expr record is.*/
   short type;
} EXPR_RECORD;

/*! An array of strings used in the program. Includes temp and declared IDs. */
char ** SymbolTable;
int L_SymbolTable;
int Cap_SymbolTable;

#pragma endregion typedefs


/* Declarations for initializing and de-initializing the symbol table. */
#pragma region lifecycle

/*! Initializes the symbol table to the start capacity. */
void SymbolTable_Init();

/*! Frees the symbol table and any strings therein. */
void SymbolTable_DeInit();

#pragma endregion lifecycle

/* Declarations for functions that deal with the symbol table memory allocation. */
#pragma region symbol_buffer

/*! Reallocates a new symbol table to one twice as large. Called when capacity has been exceeded. */
void SymbolTable_Expand();

/*! Tests if symbol table length + 1 >= the capacity. If so, expands SymbolTable. */
void SymbolTable_TestExpand();

#pragma endregion symbol_buffer

#pragma region utility
/*! 
   Checks if a symbol exists in the symbol table.
   \param symbol The string to check.
   \return 1 if it exists in the table. 0 otherwise. */
short SymbolTable_Lookup(char *symbol);
/*! 
   Adds a symbol to the symbol table.

   It will malloc a new area in the heap for the string and copy the symbol being entered to the new string before adding the new string to the SymbolTable.
   
   It will not use the parameter string passed directly. (A new one will be made. )

   \param symbol The string to add to the symbol table. 
*/
void SymbolTable_Enter(char *symbol);

/*!
   CheckID checks to see if an ID exists

   Uses enter and generate to create the symbol if it does not exist

   \param symbol the string of an ID to see if exists
 */
void SymbolTable_CheckID(char *symbol);

 /*!
   Generate writes five strings to the target file. 

   Spaces will be added between the strings. 

   The file should already be open for writing. 

   It will add a ';' to the end of the line.

   \param targetFile The file to write. It should already be open for writing. 
   \param first The first string to write.
   \param second The second string to write.
   \param third The third string to write.
   \param fourth The fourth string to write.
   \param fifth The fifth string to write.

*/
void SymbolTable_Generate(FILE * targetFile, char * first, char * second, char * third, char * fourth, char * fifth);

/*!
   GetTemp gets the next temporary ID string. This will start with "Temp".

   The string be malloced onto the heap. Take care to free this when you are done with it. 

   eg: "Temp1", "Temp2", "Temp3" and so forth

   \returns A pointer to the newly malloced char array.
 */
char * SymbolTable_GetTemp();

#pragma endregion utility

#pragma region parser_actions

/*!

      Will return a new OP_RECORD.

      It will malloc a new string as the OP_RECORD's .data element. 

      The string will be a copy of the contents of the buffer passed in.
      \param buffer A pointer to a string.
      \param l_buffer The length of the buffer
      \returns  it returns the operation record
      */
OP_RECORD Process_Op();

/*!
       process_id
       generates the code for the ID semantic record
       it creates an expression record and sets its kind to IDEXPR
       it sets its string to the contents of the token buffer which is the ID
       \returns mit then returns the id expression record
      */
      
short Process_Id();

/*!
       #process_literal
      - generates the code for the literal semantic record
      - it creates an expression record and sets its kind to LITERALEXPR
      - it sets its string to the contents of the token buffer which is the number
      - we can keep the number as a string since it will eventually be outputted via a C printf
      \returns- it then returns the literal expression record
      */


short Process_Literal(short LHS_Expr , short RHS_Expr );

/*- #gen_infix
      - generates the code for the infix semantic record
      - it will accept two expression records for the left hand side and one for the right hand side and an operation record
      - it creates third expression record and sets its kind to TEMPEXPR
      - it sets its string to a new TempID by using gettemp
      - it generates C code with generate for the assignment of the left expression right expression to the Temp
      - i.e. Temp5 = X + 7;
      - \returns it then returns the temp expression record
      - this process will always have pairs of params condensing down to one new temporary which build a more complex expression
      */


short Gen_Infix();

#pragma endregion parser_actions
/*
Functions:


 Action Functions
 - #start
    - initialization of intermediate c code file and other initializations
        - write a descriptive heading to the listing and output files
        - initialize symbol table
        - inti temp counter
        - init line counter

 - #finish
    - write descriptive closing to the listing and output files
    - catenate the files together

 - #assign
    -genrates the code for thassigent statement
    - receive two expression records one for the LHS and RHS
    -will call generate passing the two contents of the expression records along with the ' = '
    so that a correct C assigment is created

 - #read_id
    - generates the code for the read statement
    - receive an expression record and generate a scanf statement
    - "\nscanf("%d", &x);"

 - #write_expr
    - generates for print statement
    - "\nprintf("%d\n",x);"
*/

#endif