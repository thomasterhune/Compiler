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
        if(strcmp(SymbolTable[index], symbol) == 0){
            exists = 1;
        }
        index++;
    }

    return exists;
}

void SymbolTable_Enter(char *symbol){
    SymbolTable_TestExpand();
    SymbolTable[L_SymbolTable] = malloc(sizeof(char) * strlen(symbol));
    strcpy(SymbolTable[L_SymbolTable], symbol);
    L_SymbolTable+=1;
}

void SymbolTable_CheckID(char *symbol){
    if(!SymbolTable_Lookup(symbol)) {
        SymbolTable_Enter(symbol);
    }    
}

void SymbolTable_Generate(FILE * targetFile ,char * first, char * second , char * third , char * fourth , char * fifth){

    fprintf(targetFile, "%s %s %s %s %s;", first, second, third, fourth, fifth);
}

char * tempPrefix = "_temp_";

char * SymbolTable_GetTemp(){
    static int tempnum = 1;
    int len = (int) log10( (double) tempnum) + 1;
    char *numstring = malloc(len * sizeof(char) + 1);
    char *tempstring = malloc((len + strlen(tempPrefix)) * sizeof(char) + 1);
    
    itoa(tempnum, numstring, 10);
    strcpy(tempstring, tempPrefix);
    strcat(tempstring, numstring);
    tempnum++;
    free(numstring);
    
    return tempstring;
}

#pragma endregion Utility

#pragma region action

OP_RECORD Process_Op() {
}

short Process_Id(){

}

short Process_Literal(short LHS_Expr , short RHS_Expr ){

}

short Gen_Infix(){
    
}


#pragma endregion action

