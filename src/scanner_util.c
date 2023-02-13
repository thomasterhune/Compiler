#include "scanner_util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *resizeBuffer(char *bufferToDouble, int newSize){
    char *newBuffer = malloc(newSize * sizeof(char));
    strcpy(newBuffer, bufferToDouble);
    free(bufferToDouble);
    return newBuffer;
}

char* refreshBuffer(char * bufferToRefresh, int newSize){
    char *newBuffer = malloc(sizeof(char) * newSize);
    free(bufferToRefresh);
    return newBuffer;
}

int skipWhitespace(char * buffer, int * index) {
    char test = buffer[*index];
    int start = *index;
    while(test == ' ' || test == '\t') {
        *index += 1;
        test = buffer[*index];
    }
    return *index - start; /* Return num chars read by checking buffer offset */
}

char* extractWord(char* buffer, int * index, char* boundrychars, int l_boundrychars) {
    int start = *index;
    int offset = start;
    char test = buffer[offset];
    short at_boundry = 0;
    while(!at_boundry) {
        at_boundry = charIn(test, boundrychars, l_boundrychars);
        offset+=1;
        test = buffer[offset];
    }
    int size_neword = offset-start+1;
    char * new_word  = malloc( size_neword * sizeof(char)) ;
    int i = 0;
    while(start < offset - 1) {
        new_word[i] = buffer[start];
        i++;
        start++;
    }
    *index = offset - 1;
    new_word[i] = '\0';
    return new_word;
}

char* extractInt(char* buffer, int* index) {
    int start = *index;
    int offset = start;
    char test = buffer[offset];
    short at_boundry = 0;
    while(!at_boundry) {
        at_boundry = test >= '0' && test <= '9';
        offset+=1;
        test = buffer[offset];
    }
    int size_neword = offset-start+1;
    char * new_word  = malloc( size_neword * sizeof(char)) ;
    int i = 0;
    while(start < offset - 1) {
        new_word[i] = buffer[start];
        i++;
        start++;
    }
    *index = offset - 1;
    new_word[i] = '\0';
    return new_word;
}

short charIn(char test, char* charset, int l_charset) {
    int i = 0;
    char check = charset[i];
    short ret_val = 0;
    while(i < l_charset) {
        if(!ret_val) {
            check = charset[i];
            ret_val = test == check;
        }
        i++;
    }
    return ret_val;
}
