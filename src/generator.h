#ifndef generator_h
#define generator_h

#include <stdlib.h>
#include <stdio.h>

#include "tokens.h"
#include "file_util.h"

#pragma region defines

#define SYMBOL_TABLE_START_CAPACITY 20

#pragma endregion defines

#pragma region typedefs

/*! OP_RECORD.data is a 3 character array; max two for the operation string plus a null terminator. */
typedef struct {
   char* data;
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
   /* Indicates whether the reference memory has been freed or not */
   short freed;
} EXPR_RECORD;

/*! An array of strings used in the program. Includes temp and declared IDs. */
char ** SymbolTable;
int L_SymbolTable;
int Cap_SymbolTable;

#pragma endregion typedefs

#pragma region expr_helpers

/*!
   Gets a new expression record.
*/
EXPR_RECORD ER_New();

/*!
   Frees the expression record's reference parameter, if it hasn't been set.
*/
void ER_Free(EXPR_RECORD * expr_record);

/*!
   Sets the reference parameter of an expression record, and sets the indicator whether it has been freed to false.
   \param malloced_string The char * to a malloced string to place in the expr_record
   \param expr_type The type of this expression
*/
void ER_Populate(EXPR_RECORD * expr_record, char * malloced_string, short expr_type);


#pragma endregion expr_helpers


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
   \param outfile The file to print the declaration to if the symbol does not already exist
 */
void SymbolTable_CheckID(char *symbol, FILE * outfile);

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

#pragma region debug

void SymbolTable_DBPrintAll();

#pragma endregion debug

#pragma region compile

/*! Asks the user about compilation and running. */
void askAboutCompilation(char * outfilename);

/*! Gets an available name for the output file. */
char * getTempExecutable();

/*! Prompts the user if they would like to compile. */
short promptCompile(char * outfilename);

/*! Compiles the program. */
char * compile(char * outfilename);

/*! Asks user if they want to run their binary */
short promptRun();

/*! Runs the binary they made. */
void run(char * exename);


#pragma endregion compile


#endif