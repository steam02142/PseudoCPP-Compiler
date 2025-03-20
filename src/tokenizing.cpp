#include "../include/tokenizing.h"

int tokenize(token tokens[])
{
    int pos = 0;
    string input;
    string strlit;

    regex string("\"[^\"]+\"");
    regex integer("[0-9]+");
    regex real("[0-9]+\\.[0-9]+");
    regex ident("[a-zA-Z]+");

    cout << "Beginning scanning:\n\n";

    // In order, check: keywords, integers, reals, strings, identifiers
    //      Later add: booleans, arrays
    while (cin >> skipws >> input) {
        

        if (hanldeKeyword(input, tokens, pos)){
            continue; // move to next token
        }

        // check if it's a string
        if(input[0] == '"') {
            cout << "input " << input << endl; 
            strlit = input;
            while(input[input.length() - 1] != '"') {
                if(cin.fail() || cin.eof()){
                    break;
                }
                cin >> skipws >> input;
                strlit += " " + input;
            }
            if(regex_match(strlit, string)) {
                storeToken(tokens, StrLit, strlit, pos);
            } else if (cin.fail() || cin.eof()) {
                cerr << "Scanning error: unclosed string after token in position " << pos - 1 << endl;
            } else {
                cerr << "Scanning error: string has more than 2 quotes after token in position " << pos - 1 << endl;
                cerr << "\tstring content: " << strlit << endl;
            }
            continue;
        }

        // check if it's an integer
        else if (regex_match(input, integer)) {
            storeToken(tokens, IntLit, input, pos);
        } else if (regex_match(input, real)) {
            storeToken(tokens, RealLit, input, pos);
        } else if (regex_match(input, ident)) {
            storeToken(tokens, Identifier, input, pos);
        } else {
            cout << "Scanning error: \"" << input << "\" invalid token (found after the token in postition " << pos - 1 << ")" << endl;
        }
        
    }

    return pos;
}

bool hanldeKeyword(string input, token tokens[], int& pos)
{
    for (unsigned int keypos = Function; keypos <= EndFunction; keypos++) {
        string keyword = ttypeTostr(keypos);
        if (keyword == input) {
            storeToken(tokens, keypos, input, pos);
            return  true;
        }
    }

    // input is not a keyword
    return false;
}

void storeToken(token tokens[], unsigned int ttype, string input, int& pos)
{
    tokens[pos].ttype = ttype;
    tokens[pos].content = input;
    tokens[pos].pos = pos;

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
        case StrLit: return "strlit";
        case IntLit: return "intlit";
        case RealLit: return "reallit";
        case Identifier: return "identifier";
        case EndFunction: return "ENDFUNCTION";
        default: return "Invalid";
}

}

void printTokens(token tokens[], int size)
{
    cout << "\nTotal valid tokens: " << size << endl << endl;
    for (int pos = 0; pos < size; pos++) {
        cout << "Token " << pos << ": " << ttypeTostr(tokens[pos].ttype);
        cout << ", input: " << tokens[pos].content << endl;
    }
    cout << "\n---end of tokens---\n\n";
}