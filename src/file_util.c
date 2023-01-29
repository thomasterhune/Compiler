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
        if(fileExists(new_name)) {
            backupFile(new_name);
        }
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

char * getString() {
    short max_characters = 49;
    short counter = 0;
    char * temp_string = (char *) malloc (sizeof(char) * (max_characters + 1));
    char temp_char;
    
    do{
        temp_char = getchar();
        if(temp_char != '\n') {
            temp_string[counter] = temp_char;
            counter = counter + 1;
        } else {
            temp_string[counter] = '\0';
            counter = counter + 1;
        }
    }while(
       temp_char != '\n' &&
       temp_char != '\0' &&
       counter <= max_characters ); 

    temp_string = (char *) realloc (temp_string, (sizeof(char) * (counter + 4)));
    return temp_string;
}

#pragma endregion prompts
