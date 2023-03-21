#ifndef tompiler_h
#define tompiler_h
/*!
    \file tompiler.h
    \brief Tompiler lifecycle functions

    Declarations for Tompiler functions. These are the lifecycle functions (Init, Execute, DeInit) and some associated pretty-printing functions.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date March 2023
*/

#include "file_util.h"
#include "compfiles.h"

#include "scan.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*!
    Tompiler_Init the program by initializing the modules needed by Tompiler modules in the correct order. 
*/
void Tompiler_Init();

/*!
    Tompiler_Execute with command line args
*/
void Tompiler_Execute(int argc, char* argv[]);

/*!
    Tompiler_DeInit deinitializes Tompiler modules. 
*/
void Tompiler_DeInit();

/*! 
    Enables pretty printing. (Virtual Terminal Sequences)
 */
void Enable_PrettyPrint();


/*!
    Prints the goodbye message. 
*/
void Tompiler_Goodbye();
/*!
    Prints the hello message. 
*/
void Tompiler_Hello();

/*!
    Prints the compilation result. Prints red if compilation failed, yellow if it succeeded with errors, and red if it failed.
    \param err The result of Parse_SystemGoal; will be 1 if compilation failed, 0 otherwise. 
    \param listing The listing file to print to.
*/
void Tompiler_PrintResult(short err, FILE * listing);





#endif
