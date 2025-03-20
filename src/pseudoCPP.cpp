#include "../include/tokenizing.h"
#include "../include/parsing.h"
#include "../include/context_checker.h"


int main()
{
    token tokens[MaxTokens];
    int numTokens;

    numTokens = tokenize(tokens);

    // Debugging
    parse(tokens, numTokens);

    initializeScope();
    //printProcedures();
    insertProcedure("proc1");
    insertProcedure("proc2");
    insertProcedureParam("param1", 1);
    insertProcedureParam("param2", 2);

    printProcedures();

}