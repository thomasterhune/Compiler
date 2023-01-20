/**
 * 
 * Author: Tom Terhune
 * Author: Karl Miller
 * Author: Anthony Stepich
 * 
 * program 1 for CSC 460.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_util.h"

short fileExists(const char * filename) {
    FILE * file = fopen(filename, "r");
    if(file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

int filenameHasExtension(const char * filename) {
    char * pointer_to_last_period = strrchr(filename, '.');
    int return_value = 0;
    if(pointer_to_last_period == NULL) {
        return_value = FILENAME_HAS_NO_PERIOD;
    } else {
        if(pointer_to_last_period == filename) {
            /* If they have the same address, the filename string starts with '.' and is invalid. */
            return_value = FILENAME_STARTS_WITH_PERIOD;
        } else {
            char * first_occurence;
            first_occurence = strchr(filename, '.');
            if(first_occurence == filename) {
                return_value = FILENAME_STARTS_WITH_PERIOD;
            } else {
                int period_index = pointer_to_last_period - filename;
                int string_length = strlen(filename);
                if(period_index == string_length - 1) {
                    /* If the string ended with a period, it is invalid. */
                    return_value = FILENAME_ENDS_IN_PERIOD;
                } else {
                    /* Otherwise, the filename is well formed and we can return the index of the period. */
                    return_value = period_index;
                }
            }
        }
    }
    return return_value;
}


short promptUserOverwriteSelection() {
    int user_selection = 0;
    printf("That file already exists. What would you like to do?\n");
    printf("(1) : Reenter a filename.\n");
    printf("(2) : Overwrite the file.\n");
    printf("(3) : Use the default filename.\n");
    printf("(4) : Terminate the program.\n");
    scanf("%d", &user_selection);
    if(
        user_selection != USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED &&
        user_selection != USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE &&
        user_selection != USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME &&
        user_selection != USER_OUTPUT_TERMINATE_PROGRAM
        ) {
            user_selection = USER_OUTPUT_TERMINATE_INVALID_ENTRY;
        }
    return (short) user_selection;
}