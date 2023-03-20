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


#include "tompiler.h"

/*!
   Program entry point.
*/
int main(int argc, char *argv[]) {
    Tompiler_Init();
    Tompiler_Execute(argc, argv);
    Tompiler_DeInit();
    return 2023;
 }