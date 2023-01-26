/**
  * 
  * Author: Tom Terhune 
  * 	E-mail: ter1023@pennwest.edu
  * Author: Karl Miller
  * 	E-mail: mil7865@pennnwest.edu
  * Author: Anthony Stepich
  * 	E-mail: ste4864@pennwest.edu
  * 
  * Program 1 - fileopen
  * Group 3
  * CSC 460 - Language Translation
  */

 #include "file_util.h"
 #include "compfiles.h"
 #include <stdio.h>
 #include <string.h>
 #include <stdbool.h>
 #include <stdlib.h>


 void printCmp (const char *__format, ...) {
    char * new_string = malloc(sizeof(char) * strlen(__format)+20);
    strcpy(new_string, "\nTOMPILER :: ");
    strcat(new_string, __format);
    printf(new_string);
 }

 int main(int argc, char *argv[]) {
    CompFiles_Init();
    short terminate_requested = 0;
    if(argc < 2) {
        terminate_requested = CompFiles_ValidateFiles(NULL, NULL);
    } else if(argc < 3) {
        terminate_requested = CompFiles_ValidateFiles(argv[1], NULL);
    } else {
        terminate_requested = CompFiles_ValidateFiles(argv[1], argv[2]);
    }
    if(terminate_requested != 1) {
        CompFiles_GenerateTempFile();
        CompFiles_CopyInputToOutputs();
    }
    printf("\n\ndone running... for now.");
    CompFiles_DeInit();
 }