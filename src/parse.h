#ifndef parser_h
#define parser_h

/*! Called by main. Begins the parsing process.

Production 40. <system goal> -> <program> SCANEOF
*/
short Parse_SystemGoal();

/*! Called by SystemGoal. Parses the program, then matches a SCANEOF token. 

Production 1: <program> -> BEGIN <statement list> END
*/
short Parse_Program();

/*! Called by Program. Parses a statement, then possibly processes an additional statement list. 

Production 2: <statement list> -> <statement> {<statement list>}

*/
short Parse_StatementList();

/*! Called by Program, parses a list of statements
    Production 3: <statement> -> ID := <expression>;
    Production 4: <statement> -> READ ( <id list> );
    Production 5: <statement> -> WRITE ( <expr list> );
    Production 6: <statement> -> IF ( <condition> )THEN <StatementList> <IFTail>
    Production 9: <statement> -> WHILE ( <condition>)  {<StatementList>} ENDWHILE

*/
short Parse_Statement();

/*!
    Production 12: <expression> -> <term> {<add op> <term>}
*/
short Parse_Expression();

/*!
    Production 11: <expr list> -> <expression> {, <expr list>}
*/
short Parse_ExpressionList();

/*!
    Production 7: <IFTail> -> ELSE <StatementList> ENDIF
    Production 8: <IFTail> -> ENDIF
*/
short Parse_IfTail();

/*!
    Production 22: <condition> 6 <addition> {<rel op> <addition>}
*/
short Parse_Condition();

/*!
    Production 10: <id list> -> ID {,<id list> }
*/
short Parse_IDList();

/*
    Production 13: <term> -> <factor> {<mult op> <factor>}
*/
short Parse_Term();

/*
    Production 14: <factor> -> ( <expression> )
    Production 15: <factor> -> - <factor>
    Production 16: <factor> -> ID
    Production 17: <factor> -> INTLITERAL
*/
short Parse_Factor();

/*
    Production 18: <add op> -> +
    Production 19: <add op> -> -
*/
short Parse_AddOP();

/*
    Production 20: <mult op> -> *
    Production 21: <mult op> -> /
*/
short Parse_MultOP();

/*
    Production 22: <condition> -> <addition> {<rel op> <addition>}
*/
short Parse_Condition();

/*
    Production 23: <addition> -> <multiplication> {<add op> <multiplication>}
*/
short Parse_Addition();

/*
    Production 24: <multiplication> -> <unary> { <mult op> <unary>}
*/
short Parse_Multiplication();

/*

*/
short Parse_Unary();

/*

*/
short Parse_LPrimary();

/*

*/
short Parse_RelOP();
#endif