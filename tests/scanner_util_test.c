#include <stdio.h>
#include "CuTest.h"
#include "scanner_util.h"
#include <stdlib.h>
#include <string.h>


void test_doubleBuffer(CuTest *tc){

    char * test_str = malloc(sizeof(char) * strlen("hello world!"));
    strcpy(test_str, "hello world!");
    int start_size = strlen(test_str);
    char * new_string = resizeBuffer(test_str, start_size*2);
    new_string[start_size*2-1] = 'x'; /* Should NOT crash program. */
    CuAssertIntEquals_Msg(tc, "This should be reached.",1,1);
    

}

void test_refreshBuffer(CuTest *tc){
    char *test_string = malloc(sizeof(char) * 150);
    int new_size = 100;
    char *new_string = refreshBuffer(test_string, new_size);
    new_string[99] = 'c'; /*Should not crash program*/
    CuAssertIntEquals_Msg(tc, "The old char array should be freed and new char array should be 200 bytes", 1, 1);
}

void test_skipWhitespace(CuTest *tc) {
    char * buf = "s     a buffer\n";
    int index = 0;
    skipWhitespace(buf, &index);
    CuAssertIntEquals_Msg(tc, "if starts on a char, should end on a char.", 0, 0);
    index += 1;
    skipWhitespace(buf, &index);
    CuAssertIntEquals_Msg(tc, "should be at 'a'", 1, buf[index] == 'a');
    skipWhitespace(buf, &index);
    CuAssertIntEquals_Msg(tc, "should be at 'a'", 1, buf[index] == 'a');
    index += 1;
    CuAssertIntEquals_Msg(tc, "should be at 'b'", 1, buf[index] == 'b');
}



CuSuite *scannerUtilGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_doubleBuffer);
    SUITE_ADD_TEST(suite, test_refreshBuffer);

    return suite; 
}