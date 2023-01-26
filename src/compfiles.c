#include "compfiles.h"

void CompFiles_Init() {
    CompFiles.in = NULL;
    CompFiles.out = NULL;
    CompFiles.temp = NULL;
    CompFiles.listing = NULL;
    CompFiles.input_file_state = COMPFILES_STATE_NO_NAME_PROVIDED;
    CompFiles.output_file_state = COMPFILES_STATE_NO_NAME_PROVIDED;
    CompFiles.terminate_requested = 0;
    CompFiles.input_file_name = NULL;
    CompFiles.lastUserProvidedFilename = NULL;
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
    CompFiles_Init();
}

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



short CompFiles_ValidateOutputFile(const char * filename){
    /*
    short file_extension_parse;
    char * tempfilename = NULL;
    if(filename != NULL) {
        char * tempfilename = malloc(sizeof(char) * (strlen(filename) + 4));
        strcpy(tempfilename, filename);
    } else {
        printf("\nPlease provide an output filename: ");
        CompFiles_promptOutputFilename(tempfilename);
    }
    while(CompFiles.output_file_state != COMPFILES_STATE_NAME_NEEDS_VALIDATION && CompFiles.terminate_requested != 1)
    {
        file_extension_parse = filenameHasExtension(filename);
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
            printf("\n\t- That file doesn't exist. Please Provide valid filename: \n");

        }else {
                short file_exists = fileExists(tempfilename);
                if(!file_exists) {
                    printf("\n\t - That file doesn't exist. Please provide a new filename: ");
                    free(tempfilename);
                    tempfilename = getString();
                } else {
                    CompFiles.output_file_state = COMPFILES_STATE_NAME_VALIDATED;
                    CompFiles_LoadOutputFile(fopen(tempfilename, "w"));
                    /*CompFiles.output_file_name = tempfilename;

               }

    }

 */ 
}

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


char * CompFiles_promptOutputFilename() {
    char * outputFilename = getString();
    if(outputFilename[0] == '\0') {
        printf("No file name was entered. Defaulting to inputfilename.out");
        outputFilename = (char *) realloc(outputFilename, (sizeof(char) * (strlen(CompFiles.input_file_name) + 4)));
        strcpy(outputFilename, CompFiles.input_file_name);
        outputFilename = addExtension(outputFilename, "out");
    }
    return outputFilename;
}