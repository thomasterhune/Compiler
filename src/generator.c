#include "generator.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "console.h"


#pragma region expr_helpers

EXPR_RECORD ER_New() {
    EXPR_RECORD nex;
    nex.freed = 1;
    return nex;
}
void ER_Free(EXPR_RECORD * expr_record) {
    if(!(expr_record->freed)) {
        #ifdef DEBUG_PRINT
        printf("\n\n\t\t %%%% freeing: %s %%%%\n\n", expr_record->reference);
        #endif
        free(expr_record->reference);
        expr_record->freed = 1;
    }
}
void ER_Populate(EXPR_RECORD * expr_record, char * malloced_string, short expr_type) {
    ER_Free(expr_record);
    expr_record->reference = malloced_string;
    #ifdef DEBUG_PRINT
    printf("\n\n\t\t %%%% mallocing: %s %%%%\n\n", expr_record->reference);
    #endif
    expr_record->freed = 0;
    expr_record->type = expr_type;
}

#pragma endregion expr_helpers

#pragma region lifecycle

void SymbolTable_Init() {
   Cap_SymbolTable = SYMBOL_TABLE_START_CAPACITY;
   L_SymbolTable = 0;
   SymbolTable = malloc( sizeof(char*) * Cap_SymbolTable);
}

void SymbolTable_DeInit() {
   int i = 0;
   for(i = 0; i < L_SymbolTable; i++) {
      free(SymbolTable[i]);
   }
   free(SymbolTable);
   Cap_SymbolTable = 0;
   L_SymbolTable = 0;
}

#pragma endregion lifecycle

#pragma region symbol_buffer

void SymbolTable_Expand() {
    Cap_SymbolTable = Cap_SymbolTable * 2;
    char ** new_table = malloc(sizeof(char*) * Cap_SymbolTable);
    int i = 0;
    for(i = 0; i < Cap_SymbolTable; i++) {
        new_table[i] = SymbolTable[i];
    }
    free(SymbolTable);
    SymbolTable = new_table;
}

void SymbolTable_TestExpand() {
    if(L_SymbolTable + 1 >= Cap_SymbolTable) {
        SymbolTable_Expand();
    }
}

#pragma endregion symbol_buffer

#pragma region Utility

short SymbolTable_Lookup(char *symbol) {
    short exists = 0;
    short index = 0;

    

    while(!exists && index < L_SymbolTable){
        fflush(stdout);
        if(strcmp(SymbolTable[index], symbol) == 0){
            exists = 1;
        }
        index++;
    }

    SymbolTable_DBPrintAll();

    return exists;
}

void SymbolTable_Enter(char *symbol){
    SymbolTable_TestExpand();
    int l_symbol = strlen(symbol);
    char * new_symbol = malloc( (l_symbol+1) * sizeof(char) );
    int i = 0;
    for(i = 0; i < l_symbol; i++) {
        new_symbol[i] = symbol[i];
    }
    new_symbol[i] = '\0';
    SymbolTable[L_SymbolTable] = new_symbol;
    L_SymbolTable ++;
}

void SymbolTable_CheckID(char *symbol, FILE * outfile){
    if(!SymbolTable_Lookup(symbol)) {
        SymbolTable_Enter(symbol);
        fprintf(outfile, "\n\tint %s;", symbol);
    }   
}

void SymbolTable_Generate(FILE * targetFile ,char * first, char * second , char * third , char * fourth , char * fifth){
    fprintf(targetFile, "\n\t%s %s %s %s %s;", first, second, third, fourth, fifth);

    #ifdef DEBUG_PRINT
    printf("\n\t  GEN :  %s %s %s %s %s\n\n", first, second, third, fourth, fifth);
    #endif 
    
    fflush(targetFile);
}

char * tempPrefix = "_temp_";

char * SymbolTable_GetTemp(){
    static int tempnum = 1;
    int numstring_len = (int) log10( (double) tempnum) + 1;
    char *numstring = malloc( numstring_len * sizeof(char));
    int full_len = strlen(tempPrefix) + numstring_len + 1;
    char *tempstring = malloc(full_len * sizeof(char));
    itoa(tempnum, numstring, 10);
    strcpy(tempstring, tempPrefix);
    strcat(tempstring, numstring);
    tempstring[full_len] = '\0';
    tempnum++;
    free(numstring);
    
    return tempstring;
}

#pragma endregion Utility


#pragma region Debug

void SymbolTable_DBPrintAll() {
    #ifdef DEBUG_PRINT
    int i = 0;
    printf("\n SymbolTable: { ");
    for(i = 0; i < L_SymbolTable; i++) {
        printf("%s, ", SymbolTable[i]);
    }
    printf("}\n");
    #endif 
}

#pragma endregion Debug

#pragma region compile

void askAboutCompilation(char * outfilename) {
    short to_run = 0;

    short wants_compile = promptCompile(outfilename);

    char * built_executable = NULL; 

    if(wants_compile) {
        built_executable = compile(outfilename);
    }
    if(built_executable != NULL) {
        short to_run = promptRun();
        if(to_run == 1) {
            run(built_executable);
        }
        free(built_executable);
    }
}

/*! Prompts the user if they would like to compile. */
short promptCompile(char * outfilename) {
    short wants_compile = 0;
    printf("\n\nYour C code was generated. Would you like to compile the binary now? y/n ");
    fflush(stdin);
    char select;
    fflush(stdin);
    scanf("%c", &select);
    if(select == 10 || select == '\n') { select = 'y'; }
    while(select != 'n' && select != 'y' && select != 'N' && select != 'Y') {
        printf("\nI didn't understand. Would you like to compile the binary now? y/n");
        scanf("%c", &select);
        if(select == 10) { select = 'y'; }
    }
    if(select == 'n') {
        printf("\nYou elected not to compile the binary.");
        wants_compile = 0;
    } else {
        printf("\nYou elected to compile the program.");
        wants_compile = 1;
    }
    return wants_compile;
}

char * getTempExecutable() {
    int testnum = 0;
    char * temp_name = malloc(20 * sizeof(char));
    short temp_file_exists = 1;
    while(temp_file_exists) {
        free(temp_name);
        temp_name = malloc(20 * sizeof(char));
        sprintf(temp_name, "out%d", testnum);
        temp_file_exists = fileExists(temp_name);
        testnum++;
    }
    return temp_name;
}

char * compile(char * outfilename) {
    char * path = generateAbsolutePath(outfilename);
    char * outname = getTempExecutable();
    size_t size = strlen(path) + strlen(outname) + strlen("gcc -x c -o  \"\"") + 1;
    char * command = malloc(size * sizeof(char));
    sprintf(command, "gcc -x c -o %s \"%s\"", outname, path);
    printf("\n\n");
    CONSOLE_COLOR(FG_BRT_MAGENTA, BG_DEFAULT);
    printf("\t%s", command);
    CONSOLE_COLOR_DEFAULT();
    printf("\n");
    int error = system(command);
    if(error) {
        CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
        printf("\nFailed to compile with error code: %d", error);
        printf("\n%s", strerror(error));
        printf("\nEnsure gcc is on your %%PATH%%.");
        CONSOLE_COLOR_DEFAULT();
        free(outname);
        outname = NULL;
    } else {
        CONSOLE_COLOR(FG_BRT_GREEN, BG_DEFAULT);
        printf("\nCompiled %s.exe succesfully.", outname);
        CONSOLE_COLOR_DEFAULT();
    }
    free(command);
    free(path);
    return outname;
}

short promptRun() {
    short wants_compile = 0;
    printf("\n\nYour .exe file was generated. Would you like to run the binary now? ");
    CONSOLE_COLOR(FG_BRT_YELLOW, BG_DEFAULT);
    printf("\nWarning: This may result in dangerous behavior such as infinite loops! ");
    CONSOLE_COLOR_DEFAULT();
    printf("y/n? ");
    char select;
    fflush(stdin);
    scanf("%c", &select);
    if(select == 10 || select == '\n') { 
        select = 'n'; 
    }
    while(select != 'n' && select != 'y' && select != 'N' && select != 'Y') {
        printf("\nI didn't understand. Would you like to run the binary now? y/n");
        scanf("%c", &select);
        if(select == 10) { select = 'n'; }
    }
    if(select == 'n') {
        printf("\nYou elected not to run the binary.");
        wants_compile = 0;
    } else {
        printf("\nYou elected to run the binary.");
        wants_compile = 1;
    }
    return wants_compile;
}

void run(char * exename) {
    printf("\n\n");
    CONSOLE_COLOR(FG_BRT_MAGENTA, BG_DEFAULT);
    printf("\t%s", exename);
    CONSOLE_COLOR_DEFAULT();
    printf("\n");
    printf("\n====== output ======\n");
    int error = system(exename);
    printf("\n\n====================\n");
    if(error) {
        CONSOLE_COLOR(FG_BRT_RED, BG_DEFAULT);
        printf("\nFailed to run with error code: %d", error);
        printf("\n%s", strerror(error));
        CONSOLE_COLOR_DEFAULT();
    } else {
        CONSOLE_COLOR(FG_BRT_GREEN, BG_DEFAULT);
        printf("\nRan %s.exe succesfully.", exename);
    }
}

#pragma endregion compile