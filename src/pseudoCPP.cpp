#include "../include/tokenizing.h"
#include "../include/parsing.h"



int main()
{
    token tokens[MaxTokens];
    int numTokens;

    numTokens = tokenize(tokens);
    if (numTokens > 0){
        if(parse(tokens, numTokens)) {
            cout << "Successfully translated to C++ - output written to output.cpp" << endl;
        }
    } else {
        cerr << "Error: no valid tokens" << endl;
    }
}