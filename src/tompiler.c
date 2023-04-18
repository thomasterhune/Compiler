/*!
    \file tompiler.c
    \brief Tompiler lifecycle functions

    Definitions for Tompiler functions. These are the lifecycle functions (Init, Execute, DeInit) and some associated pretty-printing functions.

    \authors Tom Terhune, Karl Miller, Anthony Stepich
    \date March 2023
*/

/*
    NOTE: function descriptions are located in the header file instead of the .c file to enable intellisense type hints. 
*/

#include "tompiler.h"
#include "windows.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "parse.h"
#include "scan.h"

/*
-------------------
Lifecycle methods
-------------------
*/
#pragma region lifecycle

void Tompiler_Init() {
    Enable_PrettyPrint();
    Tompiler_Hello();
    CompFiles_Init();
    Scanner_Init();
    Parser_Init();
}

void Tompiler_Execute(int argc, char* argv[]) {
    short terminate;
    terminate = CompFiles_Open(argc, argv);
    if(!terminate) {
        TCompFiles * files = CompFiles_GetFiles();
        Scanner_LoadFiles(files->in, files->out, files->listing, files->temp);
        Parser_Load(files->out, files->listing, files->temp, files->output_file_name);
        Scanner_PrintLine();         /* Print the first line to the listing file. Subsequent calls to PrintLine are ultimately called by Scanner_Match when appropriate. */
        short err = Parse_SystemGoal();
        Scanner_PrintErrorSummary();
        Parser_PrintErrorSummary();
        Tompiler_PrintResult(err, files->listing);
    }
}

void Tompiler_DeInit() {
    Parser_DeInit();
    Scanner_DeInit();
    CompFiles_DeInit();
    Tompiler_Goodbye();
}

#pragma endregion lifecycle

#pragma region printing

HANDLE handle; 

void Enable_PrettyPrint() {
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode;
    GetConsoleMode(handle, &console_mode);
    console_mode |= 0x0004;
    SetConsoleMode(handle, console_mode);
}

void Tompiler_Hello() {
    printf("\n\n");
    CONSOLE_COLOR(FG_BLUE, BG_BRT_WHITE);
    printf("    TOMPILER");
    CONSOLE_COLOR(FG_BLACK, BG_BRT_WHITE);
    printf(" v 0.0.3    ");
    CONSOLE_COLOR_DEFAULT();
    printf("\n");
}

void Tompiler_Goodbye() {
    printf("\n\n");
    CONSOLE_COLOR(FG_BLUE, BG_BRT_WHITE);
    printf("    TOMPILER");
    printf(FG_BLACK);
    printf(" v 1.0.0    ");
    CONSOLE_COLOR(FG_BRT_YELLOW, BG_BLUE);
    printf("  Tompiler finished running. ");
    CONSOLE_COLOR_DEFAULT();
    printf("\n\n");
}

void Tompiler_PrintResult(short had_err_in_parse_system_goal, FILE * listing) {
    printf("\n\n");
    if(had_err_in_parse_system_goal){
        CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
        printf("Failed to compile.");
        CONSOLE_COLOR_DEFAULT();
        fprintf(listing, "\n\nFailed to compile.");
        /* Print red, compilation failed*/
    }else{
        int lex_errs = Scanner_GetLexErrCount();
        int parse_errs = Parser_GetParseErrCount();
        if(lex_errs > 0 || parse_errs > 0) {
            CONSOLE_COLOR(FG_BRT_YELLOW, BG_DEFAULT);
            printf("Compiled with errors.");
            CONSOLE_COLOR_DEFAULT();
            fprintf(listing, "\n\nCompiled with errors.");
        } else {
            CONSOLE_COLOR(FG_BRT_GREEN, BG_DEFAULT);
            printf("Compiled with no errors.");
            CONSOLE_COLOR_DEFAULT();
            fprintf(listing, "\n\nCompiled with no errors.");
        }
    }
}


#pragma endregion printing
