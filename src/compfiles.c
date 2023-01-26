#include "compfiles.h"

/*
-------------------
CompFiles lifecycle
-------------------
*/
#pragma region lifecycle

void CompFiles_Init() {
    CompFiles.in = NULL;
    CompFiles.out = NULL;
    CompFiles.temp = NULL;
    CompFiles.listing = NULL;
    CompFiles.input_file_state = COMPFILES_STATE_NO_NAME_PROVIDED;
    CompFiles.output_file_state = COMPFILES_STATE_NO_NAME_PROVIDED;
    CompFiles.terminate_requested = 0;
    CompFiles.input_file_name = NULL;
    CompFiles.output_file_name = NULL;
}

void CompFiles_DeInit() {
    if(CompFiles.in != NULL) {
        fclose(CompFiles.in);
    }
    if(CompFiles.out != NULL) {
        fclose(CompFiles.out);
    }
    if(CompFiles.temp != NULL) {
        fclose(CompFiles.temp);
    }
    if(CompFiles.listing != NULL) {
        fclose(CompFiles.in);
    }
    if(CompFiles.input_file_name != NULL) {
        free(CompFiles.input_file_name);
    }
    if(CompFiles.output_file_name != NULL) {
        free(CompFiles.output_file_name);
    }
    CompFiles_Init();
}

#pragma endregion lifecycle

/*
-----------------
CompFiles setters
-----------------
*/
#pragma region setters

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


char * CompFiles_promptInputFilename() {
    char * inputFilename = getString();

    if(inputFilename[0] == '\0') {
        CompFiles.terminate_requested = 1;
    }

    return inputFilename;
    
}

#pragma endregion setters

/*
-----------------
CompFiles prompts
-----------------
*/
#pragma region prompts

short CompFiles_ValidateInputFile(const char * filename) {
    short file_extension_parse;
    char * tempfilename = NULL; 
    if(filename != NULL) {
        char * tempfilename = malloc(sizeof(char) * (strlen(filename) + 4));
        strcpy(tempfilename, filename);
    } else {
        printf("\nPlease provide an input filename: ");
        tempfilename = CompFiles_promptInputFilename();
    }
    while(CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1) {

        file_extension_parse = filenameHasExtension(tempfilename);

        if(file_extension_parse == FILENAME_HAS_NO_PERIOD) {
            printf("\n\t- Your input file has no extension. Defaulting to .in. \n");
            char * tnewfile = addExtension(tempfilename, "in");
            free(tempfilename);
            tempfilename = tnewfile;
            file_extension_parse = filenameHasExtension(tempfilename);
        }

        if(file_extension_parse < 0 ) {
            printf("\n\t- That is not a valid filename. Input a new filename: ");
            free(tempfilename);
            tempfilename = CompFiles_promptInputFilename();
        } else {
            short file_exists = fileExists(tempfilename);
            if(!file_exists) {
                printf("\n\t - That file doesn't exist. Please provide a new filename: ");
                free(tempfilename);
                tempfilename = CompFiles_promptInputFilename();
            } else {
                CompFiles.input_file_state = COMPFILES_STATE_NAME_VALIDATED;
                CompFiles_LoadInputFile(fopen(tempfilename, "r"));
                CompFiles.input_file_name = tempfilename;
            }
        }
    }
    if(CompFiles.terminate_requested == 1) {
        printf("\n\t - Terminate program request received.\n");
    }
    
    return CompFiles.terminate_requested;
}



short CompFiles_ValidateOutputFile(const char * filename)
{
    short file_extension_parse;
    char * tempfilename = NULL;
    if(filename == NULL) 
    {
        printf("\n\t- Please enter an output filename: ");
        tempfilename = CompFiles_promptOutputFilename();
    } else
    {
        tempfilename = malloc(sizeof(char) + strlen(filename));
        strcpy(tempfilename, filename);
    }
    
    while(CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
    {
        file_extension_parse = filenameHasExtension(tempfilename);
        if(file_extension_parse == FILENAME_HAS_NO_PERIOD)
        {
            printf("\t- Your output file has no extension. Defaulting to .out.\n");
            char * tnewfile = addExtension(tempfilename,"out");
            free(tempfilename);
            tempfilename = tnewfile;
            file_extension_parse = filenameHasExtension(tempfilename);
        }

        if(file_extension_parse < 0)
        {
            printf("\n\t- That is not a valid filename. \n");
            printf("\n\t- Please enter an output filename: ");
            free(tempfilename);
            tempfilename = CompFiles_promptOutputFilename();
        }else {
                short file_exists = fileExists(tempfilename);
                if(file_exists) {
                    short user_selection;
                    user_selection = CompFiles_promptUserOverwriteSelection();
                    while(user_selection == USER_OUTPUT_TERMINATE_INVALID_ENTRY) {
                        printf("\n\t That is not a valid selection.");
                        user_selection = CompFiles_promptUserOverwriteSelection();
                    }
                    if(user_selection == USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME) {
                        printf("\n\t You've selected to use the name from the input file for the output file.");
                        free(tempfilename);
                        char * tempfilename_no_ext = removeExtension(CompFiles.input_file_name);
                        tempfilename = addExtension(tempfilename_no_ext, "out");
                        free(tempfilename_no_ext);
                        CompFiles.output_file_state = COMPFILES_STATE_NAME_VALIDATED;
                    } else if(user_selection == USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED) {
                        free(tempfilename);
                        printf("\n\tReenter your filename: ");
                        tempfilename = CompFiles_promptOutputFilename();
                    } else if(user_selection == USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE) {
                        CompFiles.output_file_state = COMPFILES_STATE_NAME_VALIDATED;
                        backupFile(tempfilename);
                        
                        CompFiles.output_file_name = tempfilename;
                    } else if(user_selection == USER_OUTPUT_TERMINATE_PROGRAM) {
                        CompFiles.terminate_requested = 1;
                    }
                } else {
                    CompFiles.output_file_state = COMPFILES_STATE_NAME_VALIDATED;
                    CompFiles.output_file_name = tempfilename;
                }
        }

    }

    if(CompFiles.terminate_requested != 1) {
        CompFiles_LoadOutputFile(fopen(tempfilename, "w"));
        printf("\n\tLoaded output file %s.", tempfilename);
    } else {
        printf("\n\t - Terminate program request received.\n");
    }

}



char * CompFiles_promptOutputFilename() {
    char * outputFilename = getString();
    printf(outputFilename);
    if(outputFilename[0] == '\0') {
        outputFilename = removeExtension(CompFiles.input_file_name);
        printf("No file name was entered. Defaulting to %s", outputFilename);
        outputFilename = addExtension(outputFilename, "out");
    }
    return outputFilename;
}


short CompFiles_promptUserOverwriteSelection() {

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
    fflush(stdin);
    return (short) user_pick;
}
#pragma endregion prompts