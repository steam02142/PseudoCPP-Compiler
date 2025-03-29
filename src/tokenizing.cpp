#include "../include/tokenizing.h"

int tokenize(token tokens[]) {
    int pos = 0;
    int currentLine = 1;
    int currentColumn = 1;
    char c;
    string currentToken;
    bool inString = false;
    bool invalidTokenDetected = false;

    cout << "Beginning scanning:\n\n";

    while (cin.get(c)) {
        if (c == '\n') {
            if (inString) {
                cerr << "Error (line " << currentLine << ", col " << currentColumn 
                     << "): Unclosed string\n";
                inString = false;
                invalidTokenDetected = true;
            }
            processToken(currentToken, tokens, pos, currentLine, currentColumn, invalidTokenDetected);
            currentLine++;
            currentColumn = 1;
        }
        else if ((c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f') && !inString) {
            processToken(currentToken, tokens, pos, currentLine, currentColumn, invalidTokenDetected);
            currentColumn++;
        }
        else if (!inString && (c == '[' || c == ']' || c == ',')) 
        {
            // handle any tokens that have been built up first
            if (!currentToken.empty()) {
                processToken(currentToken, tokens, pos, currentLine, currentColumn - currentToken.length(), invalidTokenDetected);
                currentToken.clear();
            }
             
            // process the symbol
            currentToken = c;
            processToken(currentToken, tokens, pos, currentLine, currentColumn, invalidTokenDetected);
            currentColumn++;
        }
        
        else {
            if (c == '"') {
                if (!inString) {
                    // Start of string
                    inString = true;
                } else {
                    // End of string
                    inString = false;
                }
            }
            currentToken += c;
            currentColumn++;
        }
    }

    // Handle unclosed strings or tokens when input ends
    if (inString) {
        cerr << "Error (line " << currentLine << ", col " << currentColumn 
             << "): Unclosed string\n";
        invalidTokenDetected = true;
    }

    processToken(currentToken, tokens, pos, currentLine, currentColumn, invalidTokenDetected);
    if (invalidTokenDetected) {
        return -1;
    }
    return pos;
}

void processToken(string& currentToken, token tokens[], int& pos, int currentLine, int currentColumn, bool& invalidTokenDetected) {
    if (currentToken.empty()) return;

    int tokenStartColumn = currentColumn - currentToken.length();

    if (hanldeKeyword(currentToken, tokens, pos, currentLine, tokenStartColumn)) {
        currentToken.clear();
        return;
    }

    regex intlit("-?[0-9]+");
    regex real("-?[0-9]+\\.[0-9]+");
    regex ident("[a-zA-Z]+");
    regex param("[a-zA-Z]+:");

    if (currentToken[0] == '"') {
        storeToken(tokens, TextLit, currentToken, pos, currentLine, tokenStartColumn);
        currentToken.clear();
        return;
    }
    else if (regex_match(currentToken, intlit)) {
        storeToken(tokens, IntLit, currentToken, pos, currentLine, tokenStartColumn);
    }
    else if (regex_match(currentToken, real)) {
        storeToken(tokens, RealLit, currentToken, pos, currentLine, tokenStartColumn);
    }
    else if (currentToken == "true" || currentToken == "false") {
        storeToken(tokens, BoolLit, currentToken, pos, currentLine, tokenStartColumn);
    }
    else if (regex_match(currentToken, ident)) {
        storeToken(tokens, Identifier, currentToken, pos, currentLine, tokenStartColumn);
    }
    else if (regex_match(currentToken, param)) {
        currentToken.pop_back(); // Remove colon
        storeToken(tokens, Parameter, currentToken, pos, currentLine, tokenStartColumn);
    }
    else {
        cerr << "Error (line " << currentLine << ", col " << tokenStartColumn 
             << "): Invalid token '" << currentToken << "'\n";
        invalidTokenDetected = true;
    }

    currentToken.clear();
}

bool hanldeKeyword(string input, token tokens[], int& pos, int line, int column)
{
    for (unsigned int keypos = Function; keypos <= EndFunction; keypos++) {
        string keyword = ttypeTostr(keypos);
        if (keyword == input) {
            storeToken(tokens, keypos, input, pos, line, column);
            return  true;
        }
    }

    // input is not a keyword
    return false;
}

void storeToken(token tokens[], unsigned int ttype, string input, int& pos, int line, int column)
{
    tokens[pos].ttype = ttype;
    tokens[pos].content = input;
    tokens[pos].pos = pos;
    tokens[pos].line = line;
    tokens[pos].column = column;

    pos++;
}

string ttypeTostr(unsigned int ttype)
{
    switch (ttype) {
        case Function: return "FUNCTION";
        case Main: return "MAIN";
        case EndMain: return "ENDMAIN";
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
        case Lt: return "<";
        case Gt: return ">";
        case Le: return "<=";
        case Ge: return ">=";
        case Ne: return "!=";
        case Eq: return "==";
        case And: return "&&";
        case TypeArrow: return "->";
        case Return: return "RETURN";
        case Or: return "||";
        case Not: return "!";
        case If: return "IF";
        case Else: return "ELSE";
        case EndIf: return "ENDIF";
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
        case SquareLBrack: return "[";
        case SquareRBrack: return "]";
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
        //cout << '\t' << "found on column: " << tokens[pos].column << " and line: " << tokens[pos].line << endl; 
    }
    cout << "\n---end of tokens---\n\n";
}