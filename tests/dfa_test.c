#include "dfa.h"
#include "CuTest.h"
#include "tokens.h"
#include <stdio.h>

void test_ScanFile(CuTest * tc) {
    char * fn = "tests/dfa_test_fil1.test.txt";
    FILE * file = fopen(fn, "w");
    fprintf(file, " begin \n a := BB & A;\nend\n");
    fclose(file);
    file = fopen(fn, "r");
    
    int charsRead = 0;
    int token = ERROR;
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, BEGIN, token);
    fgetc(file);
    fgetc(file);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, ID, token);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, ASSIGNOP, token);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, ID, token);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, ERROR, token);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, ID, token);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, SEMICOLON, token);
    fgetc(file);
    token = GetNextToken(file, &charsRead);
    CuAssertIntEquals(tc, END, token);

    fclose(file);
    remove(fn);

}

void test_GetNextTokenInBuffer(CuTest * tc) {
    char * buffer;
    int i = 0;
    int charsRead = 0;
    int token = 0;

    buffer = " ()+ :1 :=222 begin while     true\tend\n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, LPAREN, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, RPAREN, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, PLUSOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ERROR, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ASSIGNOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, BEGIN, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, WHILE, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, TRUEOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, END, token);

    i = 0;
    buffer = "  ;,;  = <= >= <\t> -1000 *1000 / 500 !TRUE &\n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, SEMICOLON, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, COMMA, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, SEMICOLON, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, EQUALOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, LESSEQUALOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, GREATEREQUALOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, LESSOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, GREATEROP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, MULTOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, DIVOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, NOTOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, TRUEOP, token);
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ERROR, token);


    /* Check all keywords and identifiers */
    i = 0;
    buffer = "    WRITE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, WRITE, token);
    CuAssertIntEquals(tc, i, 9);
    i = 0;
    
    buffer = "  ELSE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ELSE, token);
    i=0;

    buffer = "\t\tREAD \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, READ, token);
    i=0;
    
    buffer = " end\n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, END, token);
    CuAssertIntEquals(tc, 3, charsRead);
    i = 0;

    buffer = "BEGIN \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, BEGIN, token);
    i=0;
    buffer = "END \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, END, token);
    i=0;

    buffer = "READ \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, READ, token);
    i=0;

    buffer = "IF \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, IF, token);
    i=0;
    buffer = "THEN \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, THEN, token);
    i=0;
    buffer = "ENDIF \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ENDIF, token);
    i=0;
    buffer = "WHILE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, WHILE, token);
    i=0;
    buffer = "ID \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ID, token);
    i=0;

    buffer = " 1234005 \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, INTLITERAL, token);
    i=0;
    buffer = "NULL& \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, NULLOP, token);
    i=0;

    buffer = "ENDWHILE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, ENDWHILE, token);
    i=0;
    
    buffer = " FaLSE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, FALSEOP, token);
    i=0;

    buffer = "\t\ttRUE \n";
    token = GetNextTokenInBuffer(buffer, &i, &charsRead);
    CuAssertIntEquals(tc, TRUEOP, token);
    i=0;
}

void test_GetDFAColumn(CuTest *tc) {
    char buff[100];
    int i = 0;
    char test = 'x';
    char testcap = 'X';
    while(i < 26) {
        test = 'a' + i;
        sprintf(buff, "%c should be %d", test, i);
        CuAssertIntEquals_Msg(tc, buff, i, GetDFAColumn(test));
        test = 'A' + i;
        sprintf(buff, "%c should be %d", test, i);
        CuAssertIntEquals_Msg(tc, buff, i, GetDFAColumn(test));
        i++;
    }
    test = '\n';
    i = 43;
    CuAssertIntEquals_Msg(tc, "rec whitespace?", i, GetDFAColumn(test));
    test = ' ';
    i = 26;
    CuAssertIntEquals_Msg(tc, "rec whitespace?", i, GetDFAColumn(test));
    test = '(';
    CuAssertIntEquals_Msg(tc, "lparen?", 27, GetDFAColumn(test));
    test = ')';
    CuAssertIntEquals_Msg(tc, "rparen?", 28, GetDFAColumn(test));

}


CuSuite* dfaTestGetSuite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_GetDFAColumn);
    SUITE_ADD_TEST(suite, test_GetNextTokenInBuffer);
    SUITE_ADD_TEST(suite, test_ScanFile);
    return suite;

}