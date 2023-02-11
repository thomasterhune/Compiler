#include <stdio.h>
#include "CuTest.h"
#include "scanner.h"

void test_Scanner_Init(CuTest *tc){

}

void test_Scanner_DeInit(CuTest *tc){

}

void test_Scanner_clearBuffer(CuTest *tc){

}

void test_Scanner_expandBuffer(CuTest *tc){

}

const char * pbtFile = "scn_tbuf_poplate.txt";
void test_Scanner_populateBuffer(CuTest *tc) {
    FILE * in = fopen(pbtFile, "w");
    fprintf(in, "Something in a file.\nA new line.\na 160 character linea 160 character linea 160 character linea 160 character linea 160 character linea 160 character linea 160 character line\nSomething else, then the EOF.");
    fclose(in);
    in = fopen(pbtFile, "r");
    /* We only need the in file here. */
    Scanner_Init(in, NULL, NULL, NULL);
    
    int charsRead;
    TScanner* scanner = __GetScanner();
    
    charsRead = Scanner_populateBuffer();

    CuAssertIntEquals_Msg(tc, "There should be a 'S' at position 0", 'S', scanner->buffer[0]);
    CuAssertIntEquals_Msg(tc, "There should be a newline at position 20", '\n', scanner->buffer[20]);
    CuAssertIntEquals_Msg(tc, "There should not be an 'A' at position 21", 0, scanner->buffer[21]=='A');
    CuAssertIntEquals_Msg(tc, "The correct num of chars should have been read, to include the newline.", 21, charsRead);
    
    charsRead = Scanner_populateBuffer();

    CuAssertIntEquals_Msg(tc, "There should be an 'A' at position 0", 'A', scanner->buffer[0]);
    CuAssertIntEquals_Msg(tc, "There should be an newline at position 11", '\n', scanner->buffer[11]);
    CuAssertIntEquals_Msg(tc, "The correct num of chars should have been read, to include the newline.", 12, charsRead);

    charsRead = Scanner_populateBuffer();
    CuAssertIntEquals_Msg(tc, "The scanner size should read bigger than 100", 1, scanner->current_buff_size > 100);
    CuAssertIntEquals_Msg(tc, "There should be an newline at position 140", '\n', scanner->buffer[140]);
    CuAssertIntEquals_Msg(tc, "The correct num of chars should have been read, to include the newline.", 141, charsRead);
    
    charsRead = Scanner_populateBuffer();
    CuAssertIntEquals_Msg(tc, "There should be a 'S' at position 0", 'S', scanner->buffer[0]);
    CuAssertIntEquals_Msg(tc, "There should be an EOF at position 29", EOF, scanner->buffer[29]);
    CuAssertIntEquals_Msg(tc, "The correct num of chars should have been read, to include the EOF.", 30, charsRead);
    Scanner_DeInit();
    fclose(in);
    remove(pbtFile);



}

void test_Scanner_addChar(CuTest *tc){

}

void test_Scanner_Scan(CuTest *tc){

}

CuSuite* scannerGetSuite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_Scanner_populateBuffer);
    return suite;

}