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
  * - ::TCompFiles
  * 
  * ## About
  * 
  * Created by Group 3 for CSC-460, Language Translations with Dr. Pyzdrowski, at PennWest California. 
  * 
  * 
  */


#include "file_util.h"
#include "compfiles.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*!
   Program entry point.
*/
int main(int argc, char *argv[]) {
    CompFiles_Init();
    short terminate_requested = CompFiles_Open(argc, argv);
    if(terminate_requested != 1) {
        CompFiles_CopyInputToOutputs();
        printf("\n\t- Copied input file to all output files.\n");
    }
    CompFiles_DeInit();
    printf("\n\nDone running... for now.\n");
 }