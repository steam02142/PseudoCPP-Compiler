#include "../include/parsing.h"
#include "../include/context_checker.h"

// use templates for functinos
// template <typename T> 
// T multiply(T a, T b) {
//     return a * b;
// }

bool parse(token tokens[], int numTokens)
{
    printTokens(tokens, numTokens);
    cout << "Beginning parsing:" << endl << endl;
    // DEBUG TEST VAR
    insertVariable("x", real, 0);
    
    int pos = 0;

    initializeScope();
    
    parseGlobals(tokens, pos);

    return true;
}

bool isLiteral(token token)
{
    if(token.ttype == IntLit || token.ttype == RealLit || token.ttype == TextLit || token.ttype == BoolLit)
    {
        return true;
    }
    return false;
}

SymbolType getLiteralType(unsigned int ttype)
{
    if(ttype == IntLit){ return integer; } 
    else if (ttype == RealLit) { return real; }
    else if(ttype == TextLit) { return text; }
    else { return boolean; }
}

bool parseGlobals(token tokens[], int& current)
{
    if(tokens[current].ttype == Set) {
        current++;
        parseGlobalVars(tokens, current);
    }
}


bool parseGlobalVars(token tokens[], int& current)
{
    string identifier;
    string expression;
    if(tokens[current].ttype == Identifier) {
        identifier = tokens[current].content;
        current++;
    } else {
        cerr << "Error: expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    if(tokens[current].ttype == To) {
        identifier = tokens[current].content;
        current++;
    } else {
        cerr << "Error: expected assignment operator 'TO', but got " << tokens[current].content << endl;
        return false;
    }

    parseExpr(tokens, current, expression);
    cout << expression << endl;



}

//( e( 5 + 2) + 3 )
// returns the type that the expression evaluates to
bool parseExpr(token tokens[], int& current, string& expression)
{
    int type = -1;

    if(tokens[current].ttype == Identifier){
        // check ident exists and get type
        expression += tokens[current].content;
        if(!variableExists(tokens[current].content)) { return false; }
        type = getVariableType(tokens[current].content);
        //return type

        current++;
        return true;
    } else if(isLiteral(tokens[current])){
        // check type and return it 
        expression += tokens[current].content;
        type = getLiteralType(tokens[current].ttype);

        current++;

        // return type
        return true;
    } else if(tokens[current].ttype == LBrack){
        expression += "(";
        current++;

        if (!parseBinaryExpr(tokens, current, expression)) { return false; }; 

        // Unexpected token
        if(tokens[current].ttype != RBrack){
            cout << "Missing closing bracket " << endl;
            return false;
        } 
        
        expression += ")";
        current++;
        return true;
    } else {
        // Reached end of input without bracket
        cout << "Missing closing bracket " << endl;
        return false;
    }
    
    
    
}

bool parseBinaryExpr(token tokens[], int& current, string& expression)
{
    // expr1Type & expr2Type then compare for compatibility (in other function)

    if(!parseExpr(tokens, current, expression)) { return false; }
   
    expression += " " + tokens[current].content + " ";
    current++;
   
    if(!parseExpr(tokens, current, expression)) { return false; }
    return true;
}