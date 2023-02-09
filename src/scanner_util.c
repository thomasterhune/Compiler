#include "scanner_util.h"
#include <stdlib.h>
#include <string.h>

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
