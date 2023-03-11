#include "tompiler.h"
#include "windows.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"

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
}

void Tompiler_Execute(int argc, char* argv[]) {
    short terminate;
    terminate = CompFiles_Open(argc, argv);
    if(!terminate) {
        TCompFiles * files = CompFiles_GetFiles();
        Scanner_ScanAndPrint(files->in, files->out, files->listing, files->temp);
    }
}

void Tompiler_DeInit() {
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
    printf(" v 0.0.3    ");
    CONSOLE_COLOR(FG_BRT_YELLOW, BG_BLUE);
    printf("  Tompiler finished running. ");
    CONSOLE_COLOR_DEFAULT();
    printf("\n\n");
}

#pragma endregion printing
