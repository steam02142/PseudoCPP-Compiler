#include "../include/tokenizing.h"

int tokenize(token tokens[])
{
    int pos = 0;
    string input;
    string strlit;

    regex string("\"[^\"]+\"");
    regex intlit("-?[0-9]+");
    regex real("-?[0-9]+\\.[0-9]+");
    regex ident("[a-zA-Z]+");
    regex param("[a-zA-Z]+:");

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
                storeToken(tokens, TextLit, strlit, pos);
            } else if (cin.fail() || cin.eof()) {
                cerr << "Scanning error: unclosed string after token in position " << pos - 1 << endl;
            } else {
                cerr << "Scanning error: string has more than 2 quotes after token in position " << pos - 1 << endl;
                cerr << "\tstring content: " << strlit << endl;
            }
            continue;
        }

        // check if it's an integer
        else if (regex_match(input, intlit)) {
            storeToken(tokens, IntLit, input, pos);
        } else if (regex_match(input, real)) {
            storeToken(tokens, RealLit, input, pos);
        } else if (input == "true" || input == "false") {
            storeToken(tokens, BoolLit, input, pos);
        } else if (regex_match(input, ident)) {
            storeToken(tokens, Identifier, input, pos);
        } else if (regex_match(input, param)) { 
            // remove the colon, not used in translated language
            input.pop_back();
            storeToken(tokens, Parameter, input, pos);
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
        case To: return "TO";
        case Call: return "CALL";
        case Increment: return "INCREMENT";
        case Decrement: return "DECREMENT";
        case Add: return "+"; 
        case Sub: return "-"; 
        case Mul: return "*"; 
        case Div: return "/";
        case Rem: return "%"; 
        case TextLit: return "textlit";
        case IntLit: return "intlit";
        case RealLit: return "reallit";
        case BoolLit: return "boollit";
        case Integer: return "int";
        case Real: return "real";
        case Boolean: return "bool";
        case Text: return "text";
        case Comma: return ",";
        case LBrack: return "(";
        case RBrack: return ")";
        case LRBrack: return  "()";
        case Identifier: return "identifier";
        case Parameter: return "parameter";
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