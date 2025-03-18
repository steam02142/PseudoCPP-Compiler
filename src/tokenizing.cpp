#include "../include/tokenizing.h"

int tokenize(token tokens[])
{
    int pos = 0;
    string input;

    cout << "Beginning scanning:\n\n";

    while (cin >> skipws >> input) {
        if (hanldeKeyword(input, tokens[pos], pos)){
            continue; // move to next token
        }
        
        
    }

    return pos;
}

bool hanldeKeyword(string input, token& token, int& pos)
{
    cout << "input " << input << endl; 
    for (unsigned int keypos = Function; keypos <= EndFunction; keypos++) {
        string keyword = ttypeTostr(keypos);
        if (keyword == input) {
            cout << "found" << endl;
            storeToken(token, keypos, input, pos);
            return  true;
        }
    }

    // input is not a keyword
    return false;
}

void storeToken(token& token, unsigned int ttype, string content, int& pos)
{
    token.ttype = ttype;
    token.content = content;
    token.pos = pos;

    pos++;
}

string ttypeTostr(unsigned int ttype)
{
    switch (ttype) {
        case Function: return "FUNCTION";
        case Read: return "READ";
        case Print: return "PRINT";
        case Compute: return "COMPUTE";
        case Set: return "SET";
        case Call: return "CALL";
        case Increment: return "INCREMENT";
        case Decrement: return "DECREMENT";
        case Add: return "ADD"; 
        case Sub: return "SUB"; 
        case Mul: return "MUL"; 
        case Div: return "DIV"; 
        case EndFunction: return "ENDFUNCTION";
        default: return "Invalid";
}

}

void printTokens(token tokens[], int size)
{
    cout << "\nTotal valid tokens: " << size << endl << endl;
    for (int pos = 0; pos < size; pos++) {
        cout << "Token " << pos << ": " << ttypeTostr(tokens[pos].ttype);
        cout << ", content: " << tokens[pos].content << endl;
    }
    cout << "\n---end of tokens---\n\n";
}