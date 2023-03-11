#include "CuTest.h"
#include "test.h"
#include "stdio.h"
#include "scan.h"
#include "tokens.h"
#include "std_swapper.h"



#pragma region temp_test_files

/*
    These tests utilize temporary files.

    Call makeTempFilesWithInput to create a new set of temporary files, first filling input (in) with the parameter inp.

    Use in, out, list, and tmp where necessary.
*/

char * tmpInName = "tests/tin1.in.txt";
char * tmpOutName = "tests/tin1.out.txt";
char * tmpListName = "tests/tin1.list.txt";
char * tmpTmpName = "tests/tin1.tmp.txt";

FILE * in = NULL;
FILE * out = NULL;
FILE * list = NULL;
FILE * tmp = NULL;

void initFilesAndScannerWithInput(char * inp) {
    in = fopen(tmpInName, "w");
    fprintf(in, "%s", inp);
    fclose(in);
    in = fopen(tmpInName, "r");
    out = fopen(tmpOutName, "w");
    list = fopen(tmpListName, "w");
    tmp = fopen(tmpTmpName, "w");
    Scanner_Init();
    Scanner_LoadFiles(in, out, list, tmp);
}
void deInitScannerRemoveFiles() {
    Scanner_DeInit();
    fclose(in); fclose(out); fclose(list); fclose(tmp);
    in = NULL; out = NULL; list = NULL; tmp = NULL;
    remove(tmpInName); remove(tmpOutName); remove(tmpListName); remove(tmpTmpName);
}

#pragma endregion temp_test_files

#pragma region test_util

void LookAheadMatch(CuTest *tc, int target_token) {
    const char * tokval = Token_GetName(target_token);
    int token;
    char buf[100];
    short match;

    char out[100];

    token = Scanner_NextToken();
    sprintf(out, "Lookahead should be %s", tokval);
    CuAssertIntEquals_Msg(tc, out, target_token, token);

    match = Scanner_Match(target_token);
    sprintf(out, "Should match %s", tokval);
    CuAssertIntEquals_Msg(tc, out, 1, match);
}

#pragma endregion test_util

#pragma region tests

char * in1txt = "begin end";

void test1(CuTest *tc) {
    char buf[100];
    int token;
    short match;

    initFilesAndScannerWithInput(in1txt);

    token = Scanner_NextToken();
    CuAssertIntEquals_Msg(tc, "Lookahead 1 should be BEGIN", BEGIN, token);
    Scanner_CopyBuffer(buf);
    CuAssertStrEquals_Msg(tc, "Buffer should be 'begin'", "begin", buf);

    token = Scanner_NextToken();
    CuAssertIntEquals_Msg(tc, "Lookahead 2 should be BEGIN", BEGIN, token);
    Scanner_CopyBuffer(buf);
    CuAssertStrEquals_Msg(tc, "Buffer should be 'begin'", "begin", buf);

    match = Scanner_Match(BEGIN);
    CuAssertIntEquals_Msg(tc, "Match BEGIN should be true", 1, match);
    Scanner_CopyBuffer(buf);
    CuAssertStrEquals_Msg(tc, "Buffer should be 'begin'", "begin", buf);

    token = Scanner_NextToken();
    CuAssertIntEquals_Msg(tc, "Lookahead should be END", END, token);
    Scanner_CopyBuffer(buf);
    CuAssertStrEquals_Msg(tc, "Buffer should be 'end'", "end", buf);

    match = Scanner_Match(SCANEOF);
    CuAssertIntEquals_Msg(tc, "Match scaneof should be false", 0, match);
    Scanner_CopyBuffer(buf);
    CuAssertStrEquals_Msg(tc, "Buffer should be the same as before, 'end'", "end", buf);

    token = Scanner_NextToken();
    CuAssertIntEquals_Msg(tc, "Lookahead should be SCANEOF", SCANEOF, token);

    match = Scanner_Match(SCANEOF);
    CuAssertIntEquals_Msg(tc, "Match scaneof should be true", 1, match);

    deInitScannerRemoveFiles();
}

char * in2txt = " ( ) \n ,  + \t\t \n 1111  END    READ";


void test2(CuTest *tc) {
    StdSwapper_SetStdOut("tests/tempstdo.tmp");
    initFilesAndScannerWithInput(in2txt);
    LookAheadMatch(tc, LPAREN);
    LookAheadMatch(tc, RPAREN);
    LookAheadMatch(tc, COMMA);
    LookAheadMatch(tc, PLUSOP);
    LookAheadMatch(tc, INTLITERAL);
    LookAheadMatch(tc, END);
    LookAheadMatch(tc, READ);
    LookAheadMatch(tc, SCANEOF);
    deInitScannerRemoveFiles();
    StdSwapper_RestoreStdOut(1);
}


#pragma endregion tests

#pragma region exported

CuSuite* scannerGetSuite() {
    CuSuite * suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test1);
    SUITE_ADD_TEST(suite, test2);

    return suite;
}

#pragma endregion exported