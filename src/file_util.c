/**
 * 
 * Author: Tom Terhune
 * Author: Karl Miller
 * Author: Anthony Stepich
 * 
 * program 1 for CSC 475.
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

    /* get the index of the '.' character. */
    return return_value;
}