#include "generator.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    printf("\nPRINTING TO %d", targetFile);
    fprintf(targetFile, "\n\t%s %s %s %s %s;", first, second, third, fourth, fifth);
    printf("\n\t  GEN :  %s %s %s %s %s\n\n", first, second, third, fourth, fifth);
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
    int i = 0;
    printf("\n SymbolTable: { ");
    for(i = 0; i < L_SymbolTable; i++) {
        printf("%s, ", SymbolTable[i]);
    }
    printf("}\n");
}

#pragma endregion Debug