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



CuSuite *scannerUtilGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_doubleBuffer);
    SUITE_ADD_TEST(suite, test_refreshBuffer);

    return suite; 
}