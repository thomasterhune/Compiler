#include "test.h"
#include "tokens.h"
#include <string.h>

/* ensures tokens map to the correct names. */
void testToken_GetName(CuTest *tc) {
    const char * begin = Token_GetName(BEGIN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "BEGIN", begin);
    const char * end = Token_GetName(END);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "END", end);
    const char * read = Token_GetName(READ);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "READ", read);
    const char * write = Token_GetName(WRITE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "WRITE", write);
    const char * ift = Token_GetName(IF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "IF", ift);
    const char * then = Token_GetName(THEN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "THEN", then);
    const char * elset = Token_GetName(ELSE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ELSE", elset);
    const char * endif = Token_GetName(ENDIF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ENDIF", endif);
    const char * whilet = Token_GetName(WHILE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "WHILE", whilet);
    const char * endwhile = Token_GetName(ENDWHILE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ENDWHILE", endwhile);
    const char * id = Token_GetName(ID);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ID", id);
    const char * intliteral = Token_GetName(INTLITERAL);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "INTLITERAL", intliteral);
    const char * falseop = Token_GetName(FALSEOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "FALSEOP", falseop);
    const char * trueop = Token_GetName(TRUEOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "TRUEOP", trueop);
    const char * nullop = Token_GetName(NULLOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NULLOP", nullop);
    const char * lparen = Token_GetName(LPAREN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LPAREN", lparen);
    const char * rparen = Token_GetName(RPAREN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "RPAREN", rparen);
    const char * semicolon = Token_GetName(SEMICOLON);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "SEMICOLON", semicolon);
    const char * comma = Token_GetName(COMMA);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "COMMA", comma);
    const char * assignop = Token_GetName(ASSIGNOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ASSIGNOP", assignop);
    const char * plusop = Token_GetName(PLUSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "PLUSOP", plusop);
    const char * minusop = Token_GetName(MINUSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "MINUSOP", minusop);
    const char * multop = Token_GetName(MULTOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "MULTOP", multop);
    const char * divop = Token_GetName(DIVOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "DIVOP", divop);
    const char * notop = Token_GetName(NOTOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NOTOP", notop);
    const char * lessop = Token_GetName(LESSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LESSOP", lessop);
    const char * lessequalop = Token_GetName(LESSEQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LESSEQUALOP", lessequalop);
    const char * greaterop = Token_GetName(GREATEROP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "GREATEROP", greaterop);
    const char * greaterequalop = Token_GetName(GREATEREQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "GREATEREQUALOP", greaterequalop);
    const char * equalop = Token_GetName(EQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "EQUALOP", equalop);
    const char * notequalop = Token_GetName(NOTEQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NOTEQUALOP", notequalop);
    const char * scaneof = Token_GetName(SCANEOF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "SCANEOF", scaneof);
    const char * error = Token_GetName(ERROR);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ERROR", error);
}

void testToken_Recognize(CuTest *tc) {

    int token = Token_RecognizeKeyword("while", strlen("while"));
    CuAssertIntEquals(tc, WHILE, token);
    token = Token_RecognizeKeyword("write", strlen("write"));
     CuAssertIntEquals(tc, WRITE, token);
    token = Token_RecognizeKeyword("endwhile", strlen("endwhile"));
    CuAssertIntEquals(tc, ENDWHILE, token);
    token = Token_RecognizeKeyword("if", strlen("if"));
    CuAssertIntEquals(tc, IF, token);
    token = Token_RecognizeKeyword("ifx", strlen("ifx"));
    CuAssertIntEquals(tc, ID, token);
    token = Token_RecognizeKeyword("then", strlen("then"));
    CuAssertIntEquals(tc, THEN, token);
    token = Token_RecognizeKeyword("tHeN", strlen("then"));
    CuAssertIntEquals(tc, THEN, token);
    token = Token_RecognizeKeyword("else", strlen("else"));
    CuAssertIntEquals(tc, ELSE, token);
    token = Token_RecognizeKeyword("endif", strlen("endif"));
    CuAssertIntEquals(tc, ENDIF, token);
    token = Token_RecognizeKeyword("endwoogie", strlen("endwoogie"));
    CuAssertIntEquals(tc, ID, token);
    token = Token_RecognizeKeyword("begin", strlen("begin"));
    CuAssertIntEquals(tc, BEGIN, token);

    token = Token_RecognizeKeyword("end", strlen("end"));
    CuAssertIntEquals(tc, END, token);
    token = Token_RecognizeKeyword("read", strlen("read"));
    CuAssertIntEquals(tc, READ, token);

    token = Token_RecognizeKeyword("true", strlen("true"));
    CuAssertIntEquals(tc, TRUEOP, token);
    token = Token_RecognizeKeyword("false", strlen("false"));
    CuAssertIntEquals(tc, FALSEOP, token);
    token = Token_RecognizeKeyword("null", strlen("null"));
    CuAssertIntEquals(tc, NULLOP, token);
    
    token = Token_RecognizeKeyword("^&", strlen("^&"));
    CuAssertIntEquals(tc, ERROR, token);
    
    token = Token_RecognizeKeyword("+++---5555", strlen("+++---5555"));
    CuAssertIntEquals(tc, ERROR, token);
    
    token = Token_RecognizeKeyword("ab5", strlen("ab5"));
    CuAssertIntEquals(tc, ID, token);

}

CuSuite* tokensTestGetSuite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testToken_GetName);
    SUITE_ADD_TEST(suite, testToken_Recognize);
    return suite;
}