#include "scanner.h"
#include "scanner_util.h"

#include <stdlib.h>

/*
-------------------
Scanner lifecycle
-------------------
*/
#pragma region lifecycle

void Scanner_Init(FILE * in, FILE * out, FILE * listing, FILE * tmp) {
    scanner.col_count = 0;
    scanner.buf_pos = 0;
    scanner.line_count = 0;
    scanner.in = in;
    scanner.out = out;
    scanner.listing = listing;
    scanner.temp = tmp;
    scanner.buffer = malloc(sizeof(char) * TSCANNER_INIT_BUFF_SIZE);
    scanner.current_buff_size = TSCANNER_INIT_BUFF_SIZE;
}

void Scanner_DeInit() {
    if(scanner.buffer != NULL) {
        free(scanner.buffer);
        scanner.buffer = NULL;
    }
    scanner.in = NULL;
    scanner.out = NULL;
    scanner.listing = NULL;
    scanner.temp = NULL;
}

#pragma endregion lifecycle

/*
-------------------
Scanner buffer
-------------------
*/

#pragma region buffer

void Scanner_clearBuffer() {
    scanner.buffer = refreshBuffer(scanner.buffer, TSCANNER_INIT_BUFF_SIZE);
    scanner.current_buff_size = TSCANNER_INIT_BUFF_SIZE;
    scanner.buf_pos = 0;
}

void Scanner_expandBuffer() {
    scanner.buffer = resizeBuffer(scanner.buffer, scanner.current_buff_size*2);
}

#pragma endregion buffer

/*
----------------
Scanner logic
----------------
*/

#pragma region logic

void Scanner_Scan() {

}


#pragma endregion logic



