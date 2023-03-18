#include "dfa.h"
#include "tokens.h"
#include "scan.h"
#include <string.h>
#include <stdlib.h>

typedef enum {
    ASSIGNMENT_STATEMENT = 5,
    READ_STATEMENT = 4,
    WRITE_STATEMENT = 3,
    IF_STATEMENT = 2,
    WHILE_STATEMENT = 1,
    NO_NEXT_STATEMENT = 0,
};

short Parser_Statement();

short Parser_StatementList();

short Parser_Expression();

short Parser_ExpressionList();

short Parser_IfTail();

short Parser_Condition();

short Parser_IDList();