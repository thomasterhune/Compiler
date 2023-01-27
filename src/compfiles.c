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
    CompFiles.listing_file_state = COMPFILES_STATE_NO_NAME_PROVIDED;
    CompFiles.terminate_requested = 0;
    CompFiles.input_file_name = NULL;
    CompFiles.output_file_name = NULL;
    CompFiles.temp_file_name = NULL;
    CompFiles.listing_file_name = NULL;
}

void CompFiles_GenerateTempFile() {
    int testnum = 0;
    char * temp_name = malloc(20 * sizeof(char));
    while(CompFiles.temp == NULL) {
        free(temp_name);
        char * temp_name = malloc(20 * sizeof(char));
        sprintf(temp_name, "temp%d.tmp", testnum);
        short does_exist = fileExists(temp_name);
        if(does_exist) {
            testnum++;
        } else {
            CompFiles_LoadTempFile(fopen(temp_name, "w"));
            CompFiles.temp_file_name = temp_name;
        }
    }
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
        remove(CompFiles.temp_file_name);
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
    if(CompFiles.temp_file_name != NULL) {
        free(CompFiles.temp_file_name);
    }
    if(CompFiles.listing_file_name != NULL) {
        free(CompFiles.listing_file_name);
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
--------------------
CompFiles operations
---------------------
*/
#pragma region operations

void CompFiles_CopyInputToOutputs() {
    int next_char = 'x';
    next_char = fgetc(CompFiles.in);
    while(next_char != EOF){
        fputc(next_char, CompFiles.out);
        fputc(next_char, CompFiles.listing);
        fputc(next_char, CompFiles.temp);
        next_char = fgetc(CompFiles.in);
    }
}

#pragma endregion operations
/*
-----------------
CompFiles prompts
-----------------
*/
#pragma region prompts


short CompFiles_ValidateFiles(char * inputFilename, const char * outputFilename) {
    while(CompFiles.terminate_requested != 1 && CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED) {
        if(CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1) {
            CompFiles_ValidateInputFile(inputFilename);
        }
        if(CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1) {
            CompFiles_ValidateOutputFile(outputFilename);
        }
    }
    return CompFiles.terminate_requested;
}

short CompFiles_ValidateInputFile(char * filename) {
    short file_extension_parse;
    char * tempfilename = NULL; 
    if(filename != NULL) {
        tempfilename = malloc(sizeof(char) * (strlen(filename) + 4));
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
        printf("\nPlease provide an output filename: ");
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
        } else if(file_extension_parse < 0)
        {
            printf("\n\t- That is not a valid filename. \n");
            printf("\n\t- Please enter an output filename: ");
            free(tempfilename);
            tempfilename = CompFiles_promptOutputFilename();
        }else {
                short file_exists = fileExists(tempfilename);
                if(file_exists) {
                    short user_selection;
                    printf("\n\nAttempting to load output file: %s.", tempfilename);
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
        printf("\n\t- Loaded output file: %s.", tempfilename);
        char * listfile_nx = removeExtension(tempfilename);
        char * listfile_ex = addExtension(listfile_nx, "list");
        free(listfile_nx);
        CompFiles_ValidateListingFile(listfile_ex);
        free(listfile_ex);

    } else {
        printf("\n\t- Terminate program request received.\n");
    }

}

short CompFiles_ValidateListingFile(const char * filename)
{
    short file_extension_parse;
    char * tempfilename = NULL;
    if(filename == NULL) 
    {
        printf("\n\t- Please enter a listing filename: ");
        tempfilename = getString();
    } else
    {
        tempfilename = malloc(sizeof(char) + strlen(filename));
        strcpy(tempfilename, filename);
    }
    
    while(CompFiles.listing_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
    {
        file_extension_parse = filenameHasExtension(tempfilename);
        if(file_extension_parse == FILENAME_HAS_NO_PERIOD)
        {
            printf("\t- Your listing file has no extension. Defaulting to .list.\n");
            char * tnewfile = addExtension(tempfilename,"list");
            free(tempfilename);
            tempfilename = tnewfile;
        } else if(file_extension_parse < 0)
        {
            printf("\n\t- That is not a valid filename. \n");
            printf("\n\t- Please enter a listing filename: ");
            free(tempfilename);
            tempfilename = getString();
        }else {
                short file_exists = fileExists(tempfilename);
                if(file_exists) {
                    short user_selection;
                    printf("\n\nAttempting to load listing file: %s.", tempfilename);
                    user_selection = CompFiles_promptUserOverwriteSelection();
                    while(user_selection == USER_OUTPUT_TERMINATE_INVALID_ENTRY) {
                        printf("\n\t That is not a valid selection.");
                        user_selection = CompFiles_promptUserOverwriteSelection();
                    }
                    if(user_selection == USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME) {
                        printf("\n\t You've selected to use the name from the output file for the listing file.");
                        free(tempfilename);
                        char * tempfilename_no_ext = removeExtension(CompFiles.output_file_name);
                        tempfilename = addExtension(tempfilename_no_ext, "list");
                        free(tempfilename_no_ext);
                        CompFiles.listing_file_state = COMPFILES_STATE_NAME_VALIDATED;
                    } else if(user_selection == USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED) {
                        free(tempfilename);
                        printf("\n\tReenter your filename: ");
                        tempfilename = getString();
                    } else if(user_selection == USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE) {
                        CompFiles.listing_file_state = COMPFILES_STATE_NAME_VALIDATED;
                        backupFile(tempfilename);
                        
                        CompFiles.listing_file_name = tempfilename;
                    } else if(user_selection == USER_OUTPUT_TERMINATE_PROGRAM) {
                        CompFiles.terminate_requested = 1;
                    }
                } else {
                    CompFiles.listing_file_state = COMPFILES_STATE_NAME_VALIDATED;
                    CompFiles.listing_file_name = tempfilename;
                }
        }
    }

    if(CompFiles.terminate_requested != 1) {
        CompFiles_LoadListingFile(fopen(tempfilename, "w"));
        printf("\n\t- Loaded listing file %s.", tempfilename);
    } else {
        printf("\n\t- Terminate program request received.\n");
    }
}



char * CompFiles_promptOutputFilename() {
    char * outputFilename = getString();
    printf(outputFilename);
    if(outputFilename[0] == '\0') {
        free(outputFilename);
        char * ext_removed = removeExtension(CompFiles.input_file_name);
        printf("No file name was entered. Defaulting to %s", ext_removed);
        outputFilename = addExtension(ext_removed, "out");
        free(ext_removed);
    }
    return outputFilename;
}


short CompFiles_promptUserOverwriteSelection() {

    char user_selection;
    printf("\nThat file already exists. What would you like to do?\n\n");
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