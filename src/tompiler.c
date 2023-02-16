#include "tompiler.h"
#include "windows.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"

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
    console_mode |= 0x0200;
    SetConsoleMode(handle, console_mode);
}

void Tompiler_Hello() {
    printf("\n\n");
    wprintf(BG_BRT_WHITE);
    printf("    TOMPILER");
    wprintf(FG_BLACK);
    printf(" v 0.0.3    ");
    wprintf(BG_DEFAULT);
    wprintf(FG_DEFAULT);
    printf("\n");
}

void Tompiler_Goodbye() {
    printf("\n\n");
    wprintf(BG_BRT_WHITE);
    printf("    TOMPILER");
    wprintf(FG_BLACK);
    printf(" v 0.0.3    ");
    wprintf(BG_BLUE);
    wprintf(FG_BRT_YELLOW);
    printf("  Tompiler finished scanning. ");
    wprintf(BG_DEFAULT);
    wprintf(FG_DEFAULT);
    printf("\n\n");
}

#pragma endregion printing
