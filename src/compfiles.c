#include "compfiles.h"

/*
-------------------
CompFiles lifecycle
-------------------
*/
#pragma region lifecycle

void CompFiles_Init()
{
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

void CompFiles_GenerateTempFile()
{
    int testnum = 0;
    char *temp_name = malloc(20 * sizeof(char));
    short temp_file_made = 0;
    while (!temp_file_made)
    {
        free(temp_name);
        char *temp_name = malloc(20 * sizeof(char));
        sprintf(temp_name, "temp%d.tmp", testnum);
        short does_exist = fileExists(temp_name);
        if (does_exist == FILE_EXISTS)
        {
            testnum++;
        }
        else
        {
            temp_file_made = 1;
            CompFiles_LoadTempFile(fopen(temp_name, "w"));
            CompFiles.temp_file_name = temp_name;
            printf("\n\t- Created temp file %s.\n", temp_name);
        }
    }
}

void CompFiles_DeInit()
{
    if (CompFiles.in != NULL)
    {
        fclose(CompFiles.in);
    }
    if (CompFiles.out != NULL)
    {
        fclose(CompFiles.out);
    }
    if (CompFiles.temp != NULL)
    {
        fclose(CompFiles.temp);
        remove(CompFiles.temp_file_name);
        printf("\n\t- Removed temp file %s.\n", CompFiles.temp_file_name);
    }
    if (CompFiles.listing != NULL)
    {
        fclose(CompFiles.in);
    }
    if (CompFiles.input_file_name != NULL)
    {
        free(CompFiles.input_file_name);
    }
    if (CompFiles.output_file_name != NULL)
    {
        free(CompFiles.output_file_name);
    }
    if (CompFiles.temp_file_name != NULL)
    {
        free(CompFiles.temp_file_name);
    }
    if (CompFiles.listing_file_name != NULL)
    {
        free(CompFiles.listing_file_name);
    }
    CompFiles_Init();
}

TCompFiles* CompFiles_GetFiles() {
    return &CompFiles;
}

#pragma endregion lifecycle

/*
-----------------
CompFiles setters
-----------------
*/
#pragma region setters

void CompFiles_LoadInputFile(FILE *newInputFile)
{
    if (CompFiles.in != NULL)
    {
        fclose(CompFiles.in);
    }
    CompFiles.in = newInputFile;
}

void CompFiles_LoadOutputFile(FILE *newOutputFile)
{
    if (CompFiles.out != NULL)
    {
        fclose(CompFiles.out);
    }
    CompFiles.out = newOutputFile;
}

void CompFiles_LoadTempFile(FILE *newTempFile)
{
    if (CompFiles.temp != NULL)
    {
        fclose(CompFiles.temp);
    }
    CompFiles.temp = newTempFile;
}

void CompFiles_LoadListingFile(FILE *newListingFile)
{
    if (CompFiles.listing != NULL)
    {
        fclose(CompFiles.listing);
    }
    CompFiles.temp = newListingFile;
}

char *CompFiles_promptInputFilename()
{
    char *inputFilename = getString();

    if (inputFilename[0] == '\0')
    {
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

void CompFiles_CopyInputToOutputs()
{
    int next_char = 'x';
    next_char = fgetc(CompFiles.in);
    while (next_char != EOF)
    {
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

short CompFiles_Open(int argc, char *argv[]) {
    short terminate_requested = 0;
    if(argc < 2) {
        terminate_requested = CompFiles_AcquireValidatedFiles(NULL, NULL);
    } else if(argc < 3) {
        terminate_requested = CompFiles_AcquireValidatedFiles(argv[1], NULL);
    } else {
        terminate_requested = CompFiles_AcquireValidatedFiles(argv[1], argv[2]);
    }
    if(!terminate_requested) {
        CompFiles_GenerateTempFile();
    }
    return terminate_requested;
}

short CompFiles_AcquireValidatedFiles(char *inputFilename, const char *outputFilename)
{
    while (CompFiles.terminate_requested != 1 && CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED)
    {
        if (CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
        {
            CompFiles_AcquireValidatedInputFile(inputFilename);
        }
        if (CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
        {
            CompFiles_AcquireValidatedOutputFile(outputFilename);
        }
    }
    return CompFiles.terminate_requested;
}

short CompFiles_AcquireValidatedInputFile(char *filename)
{
    short file_extension_parse;
    char *tempfilename = NULL;
    if (filename != NULL)
    {
        tempfilename = malloc(sizeof(char) * (strlen(filename) + 4));
        strcpy(tempfilename, filename);
    }
    else
    {
        printf("\nPlease provide an input filename: ");
        tempfilename = CompFiles_promptInputFilename();
    }
    while (CompFiles.input_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
    {

        file_extension_parse = filenameHasExtension(tempfilename);

        if (file_extension_parse == FILENAME_HAS_NO_PERIOD)
        {
            printf("\nYour input file has no extension. Defaulting to .in. \n");
            char *tnewfile = addExtension(tempfilename, "in");
            free(tempfilename);
            tempfilename = tnewfile;
            file_extension_parse = filenameHasExtension(tempfilename);
        }

        if (file_extension_parse < 0)
        {
            printf("\nThat is not a valid filename. Input a new filename: ");
            free(tempfilename);
            tempfilename = CompFiles_promptInputFilename();
        }
        else
        {
            short file_exists = fileExists(tempfilename);
            if (file_exists == FILE_DOES_NOT_EXIST)
            {
                printf("\nThat file doesn't exist. Please provide a new filename: ");
                free(tempfilename);
                tempfilename = CompFiles_promptInputFilename();
            }
            else
            {
                CompFiles.input_file_state = COMPFILES_STATE_NAME_VALIDATED;
                CompFiles_LoadInputFile(fopen(tempfilename, "r"));
                CompFiles.input_file_name = tempfilename;
                printf("\n\t- Loaded input file %s.\n", tempfilename);
            }
        }
    }
    if (CompFiles.terminate_requested == 1)
    {
        printf("\n\t - Terminate program request received.\n");
    }

    return CompFiles.terminate_requested;
}

short CompFiles_AcquireValidatedOutputFile(const char *filename)
{
    short file_extension_parse;
    char *tempfilename = NULL;

    CompFiles.has_requested_default_filename = 0;

    if (filename == NULL) /* No file was given on the command line. */
    {
        printf("\nPlease provide an output filename: ");
        tempfilename = CompFiles_promptOutputFilename();
    }
    else
    {
        tempfilename = malloc(sizeof(char) + strlen(filename));
        strcpy(tempfilename, filename);
    }


    while (CompFiles.output_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
    {
        /* Loop as long as the output file is not validated and terminate is not requested. */

        /* Each loop, check the file extension validity. */
        file_extension_parse = filenameHasExtension(tempfilename);
        if (file_extension_parse == FILENAME_HAS_NO_PERIOD)
        {
            printf("\nYour output file has no extension. Defaulting to .out.");
            char *tnewfile = addExtension(tempfilename, "out");
            free(tempfilename);
            tempfilename = tnewfile;
        }
        else if (file_extension_parse < 0)
        {
            printf("\nThat is not a valid filename.");
            printf("\nPlease provide an output filename: ");
            free(tempfilename);
            tempfilename = CompFiles_promptOutputFilename();
        }
        else /* The filename was syntactically valid. */
        {
            short file_exists = fileExists(tempfilename); /* Find out if the file exists or can exist. */
            if (file_exists == FILE_EXISTS)
            {
                short is_same_as_input_file = checkIfSamePaths(tempfilename, CompFiles.input_file_name);
                if (is_same_as_input_file)
                {
                    printf("\nThe output file must be different from the input file!");
                    printf("\nPlease provide an output filename: ");
                    free(tempfilename);
                    tempfilename = CompFiles_promptOutputFilename();
                }
                else
                {
                    short user_selection;
                    printf("\n\nAttempting to load output file: %s.", tempfilename);
                    user_selection = CompFiles_promptUserOverwriteSelection();
                    while (user_selection == USER_OUTPUT_TERMINATE_INVALID_ENTRY)
                    {
                        printf("\nThat is not a valid selection.");
                        user_selection = CompFiles_promptUserOverwriteSelection();
                    }
                    if (user_selection == USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME)
                    {
                        if(CompFiles.has_requested_default_filename == 0) {
                            /* triggers the first time the user tries the default filename. */
                            printf("\nAttempting to generate output file from input file name. ");
                            free(tempfilename);
                            char *tempfilename_no_ext = removeExtension(CompFiles.input_file_name);
                            tempfilename = addExtension(tempfilename_no_ext, "out");
                            printf("Checking %s.", tempfilename);
                            free(tempfilename_no_ext);
                            CompFiles.has_requested_default_filename = 1;
                        } else {
                            /* Triggers when user tries the default filename twice. */
                            printf("\nDefault output filename was already tried! Exiting. \n");
                            CompFiles.terminate_requested = 1;
                        }
                    }
                    else if (user_selection == USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED)
                    {
                        free(tempfilename);
                        printf("\nRenter filename selected. ");
                        printf("\nPlease provide an output filename: ");
                        tempfilename = CompFiles_promptOutputFilename();
                    }
                    else if (user_selection == USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE)
                    {
                        backupFile(tempfilename);
                    }
                    else if (user_selection == USER_OUTPUT_TERMINATE_PROGRAM)
                    {
                        CompFiles.terminate_requested = 1;
                    }
                }
            }
            else if (file_exists == FILE_CANT_EXIST)
            {
                printf("\nThat is not a valid filename.");
                printf("\nPlease provide an output filename: ");
                free(tempfilename);
                tempfilename = CompFiles_promptOutputFilename();
            }
            else
            {
                CompFiles.output_file_state = COMPFILES_STATE_NAME_VALIDATED;
                CompFiles.output_file_name = tempfilename;
            }
        }
    }

    if (CompFiles.terminate_requested != 1)
    {
        CompFiles_LoadOutputFile(fopen(tempfilename, "w"));
        printf("\n\t- Created output file: %s.\n", tempfilename);
        char *listfile_nx = removeExtension(tempfilename);
        char *listfile_ex = addExtension(listfile_nx, "list");
        free(listfile_nx);
        CompFiles_AcquireValidatedListingFile(listfile_ex);
        free(listfile_ex);
    }
    else
    {
        printf("\n\t- Terminate program request received.\n");
    }
}

short CompFiles_AcquireValidatedListingFile(const char *filename)
{
    short file_extension_parse;
    char *tempfilename = NULL;
    CompFiles.has_requested_default_filename = 0;
    if (filename == NULL)
    {
        printf("\nPlease enter a listing filename: ");
        tempfilename = getString();
    }
    else
    {
        tempfilename = malloc(sizeof(char) + strlen(filename));
        strcpy(tempfilename, filename);
    }

    while (CompFiles.listing_file_state != COMPFILES_STATE_NAME_VALIDATED && CompFiles.terminate_requested != 1)
    {
        file_extension_parse = filenameHasExtension(tempfilename);
        if (file_extension_parse == FILENAME_HAS_NO_PERIOD)
        {
            printf("\nYour listing file has no extension. Defaulting to .list.\n");
            char *tnewfile = addExtension(tempfilename, "list");
            free(tempfilename);
            tempfilename = tnewfile;
        }
        else if (file_extension_parse < 0)
        {
            printf("\nThat is not a valid filename. \n");
            printf("\nPlease provide an output filename: ");
            free(tempfilename);
            tempfilename = getString();
        }
        else
        {
            short file_exists = fileExists(tempfilename);
            if (file_exists == FILE_EXISTS)
            {
                short is_same_as_input_file = checkIfSamePaths(tempfilename, CompFiles.input_file_name);
                short is_same_as_out_file = checkIfSamePaths(tempfilename, CompFiles.output_file_name);
                if (is_same_as_input_file)
                {
                    printf("\nThe listing file must be different from the input file!");
                    printf("\nPlease provide a listing filename: ");
                    free(tempfilename);
                    tempfilename = CompFiles_promptOutputFilename();
                }
                else if (is_same_as_out_file)
                {
                    printf("\nThe listing file must be different from the output file!");
                    printf("\nPlease provide a listing filename: ");
                    free(tempfilename);
                    tempfilename = CompFiles_promptOutputFilename();
                }
                else
                {
                    short user_selection;
                    printf("\nAttempting to load listing file: %s.", tempfilename);
                    user_selection = CompFiles_promptUserOverwriteSelection();
                    while (user_selection == USER_OUTPUT_TERMINATE_INVALID_ENTRY)
                    {
                        printf("\nThat is not a valid selection.");
                        user_selection = CompFiles_promptUserOverwriteSelection();
                    }
                    if (user_selection == USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME)
                    {
                        if(CompFiles.has_requested_default_filename == 0) {
                            printf("\nAttempting to generate listing file from output file name. ");
                            free(tempfilename);
                            char *tempfilename_no_ext = removeExtension(CompFiles.output_file_name);
                            tempfilename = addExtension(tempfilename_no_ext, "list");
                            free(tempfilename_no_ext);
                            printf("Checking %s.", tempfilename);
                            CompFiles.has_requested_default_filename = 1;
                        } else {
                            printf("\nDefault listing filename was already tried! Exiting. \n");
                            CompFiles.terminate_requested = 1;
                        }
                    }
                    else if (user_selection == USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED)
                    {
                        printf("\nRenter filename selected. ");
                        printf("\nPlease provide a listing filename: ");
                        free(tempfilename);
                        tempfilename = getString();
                    }
                    else if (user_selection == USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE)
                    {
                        backupFile(tempfilename);
                    }
                    else if (user_selection == USER_OUTPUT_TERMINATE_PROGRAM)
                    {
                        CompFiles.terminate_requested = 1;
                    }
                }
            }
            else if (file_exists == FILE_CANT_EXIST)
            {
                printf("\nThat is not a valid filename.");
                printf("\nPlease provide an listing filename: ");
                free(tempfilename);
                tempfilename = CompFiles_promptOutputFilename();
            }
            else
            {
                CompFiles.listing_file_state = COMPFILES_STATE_NAME_VALIDATED;
                CompFiles.listing_file_name = tempfilename;
            }
        }
    }

    if (CompFiles.terminate_requested != 1)
    {
        CompFiles_LoadListingFile(fopen(tempfilename, "w"));
        printf("\n\t- Created listing file %s.\n", tempfilename);
    }
    else
    {
        printf("\n\t- Terminate program request received.\n");
    }
}

char *CompFiles_promptOutputFilename()
{
    char *outputFilename = getString();
    if (outputFilename[0] == '\0')
    {
        free(outputFilename);
        char *ext_removed = removeExtension(CompFiles.input_file_name);
        printf("\nNo file name was entered. Defaulting to %s", ext_removed);
        outputFilename = addExtension(ext_removed, "out");
        CompFiles.has_requested_default_filename = 1;
        free(ext_removed);
    }
    return outputFilename;
}

short CompFiles_promptUserOverwriteSelection()
{

    char user_selection;
    printf("\nThat file already exists. What would you like to do?\n\n");
    printf("(1)/(n)     : Reenter a filename.\n");
    printf("(2)/(o)     : Overwrite the file.\n");
    printf("(3)/(enter) : Use the default filename.\n");
    printf("(4)/(q)     : Terminate the program.\n");
    short user_pick = USER_OUTPUT_TERMINATE_INVALID_ENTRY;
    user_selection = getchar();

    if (user_selection == '1' || user_selection == 'n')
    {
        user_pick = USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED;
    }
    else if (user_selection == '2' || user_selection == 'o')
    {
        user_pick = USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE;
    }
    else if (user_selection == '3' || user_selection == '\n')
    {
        user_pick = USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME;
    }
    else if (user_selection == '4' || user_selection == 'q')
    {
        user_pick = USER_OUTPUT_TERMINATE_PROGRAM;
    }
    fflush(stdin);
    return (short)user_pick;
}
#pragma endregion prompts
