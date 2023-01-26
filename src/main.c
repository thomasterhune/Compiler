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

 void requireInputFile() {
    short good_file = false;

    short file_extension_parse = 0;
    bool file_exists;
    char * userInput;
    printCmp("An input file is required. Please enter a valid filename: ");
    while(!good_file) {
        userInput = getString();
        file_extension_parse = filenameHasExtension(userInput);
        if(file_extension_parse == FILENAME_HAS_NO_PERIOD) {
                printCmp("\t- Your input file has no extension. Defaulting to '.in.'");
                char * newInput = addExtension(userInput, "in");
                free(userInput);
                userInput = newInput;
                file_extension_parse = filenameHasExtension(userInput);
        } 

        if(file_extension_parse > 0) {
            file_exists = fileExists(userInput);
            while(file_exists) {
                file_exists = fileExists(userInput);
                short user_decision;
                printf("\n");
                user_decision = CompFiles_promptUserOverwriteSelection();
                while (user_decision == USER_OUTPUT_TERMINATE_INVALID_ENTRY) {
                    printf("\nThat is not a valid selection.\n");
                    user_decision = CompFiles_promptUserOverwriteSelection();
                }

                if(user_decision == USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE) {

                    backupFile(userInput);
                    printf("\tSaved backup: %s", addExtension(userInput, "bak"));
                    good_file = true;
                    file_exists = false;
                } else if(user_decision == USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME) {
                    char * newInput = addExtension("default", "in");
                    free(userInput);
                    userInput = newInput;
                    file_exists = fileExists(userInput);
                } else if(user_decision == USER_OUTPUT_TERMINATE_PROGRAM) {
                    printCmp("We cannot terminate the program at this time!");
                }
            }
            good_file = true;
        } else {
            printCmp("Your filename was poorly formatted. \n\n");
            printCmp("An input file is required. Please enter a valid filename: ");
        }
    }
    printf("\n\nYour filename %s was well formatted and didn't exist.\n\n", userInput);
 }

 int main(int argc, char *argv[]) {
    if(argc < 2) {
        short result = CompFiles_ValidateInputFile(NULL);
        printf("result was %d", result);
        short result2 = CompFiles_ValidateOutputFile(NULL);
        /*requireInputFile();*/
    } else if(argc < 3) {
        /*validateInputFile(argv[1]);*/
    } else {
        /*validateInputOutputFiles(argv[1], argv[2]);*/
    }
 }