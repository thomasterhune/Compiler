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
#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*!
  Start the program by initializing the needed modules in the correct order. 
*/
void Init() {
    CompFiles_Init();
    Scanner_Init();
}

void Execute(int argc, char* argv[]) {
    short terminate;
    terminate = CompFiles_Open(argc, argv);
    if(!terminate) {
        TCompFiles * files = CompFiles_GetFiles();
        Scanner_Scan(files->in, files->out, files->listing, files->temp);
    }
}

void DeInit() {
    Scanner_DeInit();
    CompFiles_DeInit();
}

/*!
   Program entry point.
*/
int main(int argc, char *argv[]) {
    Init();
    Execute(argc, argv);
    DeInit();
    printf("\n\nDone running... for now. 🧑\n");
    return 2023;
 }