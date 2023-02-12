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

void test_charin(CuTest *tc) {

    short actual = charIn('a', (char[2]){'a','b'}, 2);
    CuAssertIntEquals(tc, 1, actual);
    actual = charIn('a', (char[1]){'b'}, 1);
    CuAssertIntEquals(tc, 0, actual);
    char boundries[8] = {' ', ';', '+', '-', '/', '=', '*', '\t' };
    int l = 8;
    actual = charIn('\t', boundries, l);
    CuAssertIntEquals(tc, 1, actual);
    actual = charIn(' ', boundries, l);
    CuAssertIntEquals(tc, 1, actual);
    actual = charIn(';', boundries, l);
    CuAssertIntEquals(tc, 1, actual);

}

void test_extractWord(CuTest *tc) {
    char * boundrychars = "\t+-;* \0";
    int l_boundry = 6;
    char* buffer = "hello   buffer+three\tkeyword;a-b*c    ";

    int index = 0;
    char * hello = extractWord(buffer, &index, boundrychars, l_boundry);
    CuAssertStrEquals(tc, "hello", hello);
    free(hello);

    index += 3;
    char * wbuffer = extractWord(buffer, &index, boundrychars, l_boundry);
    CuAssertStrEquals(tc, "buffer", wbuffer);
    free(wbuffer);

    index += 1;
    char * wthree = extractWord(buffer, &index, boundrychars, l_boundry);
    CuAssertStrEquals(tc, "three", wthree);
    free(wthree);

    index += 1;
    char * wkeyword = extractWord(buffer, &index, boundrychars, l_boundry);
    CuAssertStrEquals(tc, "keyword", wkeyword);
    free(wkeyword);


    
}


CuSuite *scannerUtilGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_doubleBuffer);
    SUITE_ADD_TEST(suite, test_refreshBuffer);
    SUITE_ADD_TEST(suite, test_charin);
    SUITE_ADD_TEST(suite, test_extractWord);

    return suite; 
}