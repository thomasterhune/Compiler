#include "CuTest.h"
#include "test.h"
#include "tokens.h"
#include "generator.h"
#include <string.h>
#include <stdio.h>

void testCNames(CuTest *tc) {
    const char * c_val = Token_CTranslate(LESSEQUALOP);
    CuAssertStrEquals_Msg(tc, "getting the c-string for LESSEQUALOP should be '<=' ", "<=", c_val);
    c_val = Token_CTranslate(NOTEQUALOP);
    CuAssertStrEquals_Msg(tc, "getting the c-string for NOTEQUALOP should be '!=' ", "!=", c_val);
}

void testLookupEnterCheckID(CuTest *tc) {
    SymbolTable_Init();

    char *string = "END";

    SymbolTable_CheckID(string);

    CuAssertStrEquals_Msg(tc, "the first string in symbol table should be END", SymbolTable[0], string);

    CuAssertIntEquals_Msg(tc, "symbol table length should be 1", 1, L_SymbolTable);

    short lookup_result = SymbolTable_Lookup(string);

    CuAssertIntEquals_Msg(tc, "Symbol table should return 1 if string already added", 1, lookup_result);

    SymbolTable_CheckID(string);

    CuAssertIntEquals_Msg(tc, "the same string should not change size of symbol table", 1, L_SymbolTable);

    string = "HelloWorld";

    SymbolTable_CheckID(string);

    CuAssertIntEquals_Msg(tc, "a new string should change the size of the symbol table", 2, L_SymbolTable);

    string = "NotYetIn";

    CuAssertIntEquals_Msg(tc, "A string not in the symbol table should return 0 from lookup", 0, SymbolTable_Lookup(string));

    SymbolTable_DeInit();
}


void testSymbolTableGetTemp(CuTest *tc) {

    char * first_temp = SymbolTable_GetTemp();
    CuAssertStrEquals_Msg(tc, "first temp should be ", "_temp_1", first_temp);
    free(first_temp);

    char * second_temp = SymbolTable_GetTemp();
    CuAssertStrEquals_Msg(tc, "second temp should be _temp_2", "_temp_2", second_temp);
}



CuSuite* generatorGetSuite() {
    CuSuite * suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testCNames);
    SUITE_ADD_TEST(suite, testLookupEnterCheckID);
    SUITE_ADD_TEST(suite, testSymbolTableGetTemp);
    return suite;
}

