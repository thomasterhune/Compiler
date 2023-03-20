/*! \file main.c
  * \brief Program entry point.
  * 
  * \authors Anthony Stepich
  * \authors Tom Terhune 
  * \authors Karl Miller
  * 
  * # Program 1 - fileopen
  * ## Group 3
  * ### CSC 460 - Language Translation
  * 
  * \mainpage TomPiler 
  * \version 0.2.5
  * 
  * ## Useful Pages
  * 
  * - compfiles.h
  * 
  * - file_util.h
  * 
  * - dfa.h
  * 
  * - tokens.h
  * 
  * - scan.h
  * 
  * - ::TCompFiles
  * - ::Scanner
  * 
  * ## About
  * 
  * Created by Group 3 for CSC-460, Language Translations with Dr. Pyzdrowski, at PennWest California. 
  * 
  * 
  */


#include "file_util.h"
#include "compfiles.h"
#ifndef tompiler_h
#define tompiler_h

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





#endif
