#include "../include/tokenizing.h"
#include "../include/parsing.h"



int main()
{
    token tokens[MaxTokens];
    int numTokens;

    numTokens = tokenize(tokens);

    // Debugging
    parse(tokens, numTokens);

}