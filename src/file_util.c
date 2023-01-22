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
#include <stdbool.h>
#include <string.h>
#include "file_util.h"


#pragma region fileops

short fileExists(const char * filename) {
    FILE * file = fopen(filename, "r");
    if(file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void backupFile(const char * filename) {
    short doesFileExist = fileExists(filename);
    if(doesFileExist == FILE_EXISTS) {
        char * new_name = addExtension(filename, "bak");
        rename(filename, new_name);
        free(new_name);
        remove(filename);
    }
}
#pragma endregion fileops

#pragma region filenames

int filenameHasExtension(const char * filename) {
    int return_v;
    /* Get a pointer to the last slash character in the filename, either a forward or backward slash. */
    char * p_last_fslash = strrchr(filename, '/');
    char * p_last_bslash = strrchr(filename, '\\');
    char * p_last_slash = ((char*) filename) - 1; /* Initialize last slash to an imaginary character preceding the filename. */
    /* Set last slash to whichever slash came last, if any. */
    if(p_last_fslash != NULL || p_last_bslash != NULL) {
        if(p_last_fslash == NULL) {
            p_last_slash = p_last_bslash;
        } else if (p_last_bslash == NULL) {
            p_last_slash = p_last_fslash;
        } else if (p_last_bslash > p_last_fslash) {
            p_last_slash = p_last_bslash;
        } else {
            p_last_slash = p_last_fslash;
        }
    }
    int slash_index = p_last_slash - filename;
    if(slash_index == strlen(filename) - 1) {
        /* if it ends in a '/', it's a directory not a file. */
        return_v = FILENAME_IS_DIRECTORY;
    } else {
        /* Get a pointer to the last period in the filename. */
        char * p_last_period = strrchr(filename, '.');
        if(p_last_period == NULL) {
            /* If there is no period at all, the filename can't have a valid extension. */
            return_v = FILENAME_HAS_NO_PERIOD;
        } else {
            /* There is at least one period... */
            int per_index = p_last_period - filename;
            if(per_index == strlen(filename) - 1) {
                /* If it ends in a '.' it is not a valid filename. */
                return_v = FILENAME_ENDS_IN_PERIOD;
            } else {
                if(p_last_slash > p_last_period) {
                    /* then the period was in the folder name, not the file name. */
                    return_v = FILENAME_HAS_NO_PERIOD; 
                }
                else if(p_last_period ==(p_last_slash + 1)) {
                    /* then the file name starts with a period. */
                    char next_char = *(p_last_period + 1);
                    if(next_char == '\0') {
                        return_v = FILENAME_IS_ONLY_PERIOD;
                    } else {
                        return_v = per_index;
                    }

                } else {
                    return_v = per_index;
                }
            }
        }
    }
    return return_v;

}

int filenameHasExtension2(const char * filename) {
    char * pointer_to_last_period = strrchr(filename, '.');
    int return_value = 0;
    if(pointer_to_last_period == NULL) {
        return_value = FILENAME_HAS_NO_PERIOD;
    } else {
        if(pointer_to_last_period == filename) {
            /* If they have the same address, the filename string starts with '.' and is invalid. */
            return_value = FILENAME_IS_ONLY_PERIOD;
            
            /* if the string starts with a period, we need to make sure that it's not part of a folder name. */

        } else {
            char * first_occurence;
            first_occurence = strchr(filename, '.');
            if(first_occurence == filename) {
                return_value = FILENAME_IS_ONLY_PERIOD;                
            } else {
                int period_index = pointer_to_last_period - filename;
                int string_length = strlen(filename);
                if(period_index == string_length - 1) {
                    /* If the string ended with a period, it is invalid. */
                    return_value = FILENAME_ENDS_IN_PERIOD;
                } else {
                    return_value = period_index;
                    /* Ensure that the last '.' was not in a parent folder's name. */
                    char * pointer_to_slash = strrchr(filename, '/');
                    if(pointer_to_slash != NULL) {
                        int forward_slash_index = pointer_to_slash - filename;
                        if(forward_slash_index > period_index) {
                            return_value = FILENAME_HAS_NO_PERIOD;
                        }
                    }
                    pointer_to_slash = strrchr(filename, '\\');
                    if(pointer_to_slash != NULL) {
                        int back_slash_index = pointer_to_slash - filename;
                        if(back_slash_index > period_index) {
                            return_value = FILENAME_HAS_NO_PERIOD;
                        }
                    }
                    /* Otherwise, the filename is well formed and we can return the index of the period. */
                }
            }
        }
    }
    return return_value;
}

char * addExtension(const char* filename, const char* extension) {
    /* reallocate memory for a string large enough to hold filename + '.' + extension. */
    char * new_string = (char *) malloc (sizeof(char) * (strlen(filename) + strlen(extension) + 1));
    strcpy(new_string, filename);
    strcat(new_string, ".");
    strcat(new_string, extension);
    return new_string;
}

char * removeExtension(const char* filename) {
    int index = filenameHasExtension(filename);
    size_t allocation_size = sizeof(char) * strlen(filename);
    char * new_string = (char *) malloc (allocation_size);
    strncpy(new_string, filename, index);
    new_string[index] = '\0';
    return new_string;
}


#pragma endregion filenames

#pragma region prompts

short promptUserOverwriteSelection() {

    char user_selection;
    printf("That file already exists. What would you like to do?\n");
    printf("(1)/(n)     : Reenter a filename.\n");
    printf("(2)/(o)     : Overwrite the file.\n");
    printf("(3)/(enter) : Use the default filename.\n");
    printf("(4)/(q)     : Terminate the program.\n");
    short user_pick = USER_OUTPUT_TERMINATE_INVALID_ENTRY;
    user_selection = getchar();

    if(user_selection == '1' || user_selection == 'n') {
        user_pick = USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED;
    } else if(user_selection == '2' || user_selection == 'o') {
        user_pick = USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE;
    } else if(user_selection == '3' || user_selection == '\n') {
        user_pick = USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME;
    } else if(user_selection == '4' || user_selection == 'q') {
        user_pick = USER_OUTPUT_TERMINATE_PROGRAM;
    }
    return (short) user_pick;
}

#pragma endregion prompts


#pragma region structs

void CompFiles_LoadInputFile(FILE * newInputFile) {
    if(CompFiles.in != NULL) {
        fclose(CompFiles.in);
    }
    CompFiles.in = newInputFile;
}

void CompFiles_LoadOutputFile(FILE * newOutputFile) {
    if(CompFiles.out != NULL) {
        fclose(CompFiles.out);
    }
    CompFiles.out = newOutputFile;
}

void CompFiles_LoadTempFile(FILE * newTempFile) {
    if(CompFiles.temp != NULL) {
        fclose(CompFiles.temp);
    }
    CompFiles.temp = newTempFile;
}

void CompFiles_LoadListingFile(FILE * newListingFile) {
    if(CompFiles.listing != NULL) {
        fclose(CompFiles.listing);
    }
    CompFiles.temp = newListingFile;
}



#pragma endregion structs