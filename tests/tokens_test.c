#include "tokens_test.h"
#include "tokens.h"
#include <string.h>

/* ensures tokens map to the correct names. */
void testTokensGetName(CuTest *tc) {
    const char * begin = Tokens_GetName(BEGIN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "BEGIN", begin);
    const char * end = Tokens_GetName(END);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "END", end);
    const char * read = Tokens_GetName(READ);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "READ", read);
    const char * write = Tokens_GetName(WRITE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "WRITE", write);
    const char * ift = Tokens_GetName(IF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "IF", ift);
    const char * then = Tokens_GetName(THEN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "THEN", then);
    const char * elset = Tokens_GetName(ELSE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ELSE", elset);
    const char * endif = Tokens_GetName(ENDIF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ENDIF", endif);
    const char * whilet = Tokens_GetName(WHILE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "WHILE", whilet);
    const char * endwhile = Tokens_GetName(ENDWHILE);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ENDWHILE", endwhile);
    const char * id = Tokens_GetName(ID);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ID", id);
    const char * intliteral = Tokens_GetName(INTLITERAL);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "INTLITERAL", intliteral);
    const char * falseop = Tokens_GetName(FALSEOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "FALSEOP", falseop);
    const char * trueop = Tokens_GetName(TRUEOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "TRUEOP", trueop);
    const char * nullop = Tokens_GetName(NULLOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NULLOP", nullop);
    const char * lparen = Tokens_GetName(LPAREN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LPAREN", lparen);
    const char * rparen = Tokens_GetName(RPAREN);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "RPAREN", rparen);
    const char * semicolon = Tokens_GetName(SEMICOLON);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "SEMICOLON", semicolon);
    const char * comma = Tokens_GetName(COMMA);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "COMMA", comma);
    const char * assignop = Tokens_GetName(ASSIGNOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ASSIGNOP", assignop);
    const char * plusop = Tokens_GetName(PLUSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "PLUSOP", plusop);
    const char * minusop = Tokens_GetName(MINUSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "MINUSOP", minusop);
    const char * multop = Tokens_GetName(MULTOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "MULTOP", multop);
    const char * divop = Tokens_GetName(DIVOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "DIVOP", divop);
    const char * notop = Tokens_GetName(NOTOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NOTOP", notop);
    const char * lessop = Tokens_GetName(LESSOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LESSOP", lessop);
    const char * lessequalop = Tokens_GetName(LESSEQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "LESSEQUALOP", lessequalop);
    const char * greaterop = Tokens_GetName(GREATEROP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "GREATEROP", greaterop);
    const char * greaterequalop = Tokens_GetName(GREATEREQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "GREATEREQUALOP", greaterequalop);
    const char * equalop = Tokens_GetName(EQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "EQUALOP", equalop);
    const char * notequalop = Tokens_GetName(NOTEQUALOP);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "NOTEQUALOP", notequalop);
    const char * scaneof = Tokens_GetName(SCANEOF);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "SCANEOF", scaneof);
    const char * error = Tokens_GetName(ERROR);
    CuAssertStrEquals_Msg(tc, "The enum ID doesn't match with the returned string. Did the order of the enums change, and does it still match up with tokensMap in tokens.c?", "ERROR", error);
}

CuSuite* tokensTestGetSuite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testTokensGetName);
    return suite;
}