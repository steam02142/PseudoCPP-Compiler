#include "../include/tokenizing.h"
#include "../include/parsing.h"



int main()
{
    token tokens[MaxTokens];
    int numTokens;

    numTokens = tokenize(tokens);
    if (numTokens > 0){
        parse(tokens, numTokens);
    }
}