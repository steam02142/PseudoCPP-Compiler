#include "../include/tokenizing.h"

int tokenize(token tokens[])
{
    int pos = 0;
    string input;

    cout << "Beginning scanning:\n\n";

    while (cin.good()) {
        cin >> skipws >> input;

        if (hanldeKeyword(input, tokens[pos], pos)){
            continue; // move to next token
        }
        
    }

    return pos;
}

bool hanldeKeyword(string input, token& token, int& pos)
{
    for (unsigned int keypos = Function; keypos <= EndFunction; keypos++) {
        string keyword = ttypeTostr(keypos);
        if (keyword == input) {
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
        case Set: return "SET";
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