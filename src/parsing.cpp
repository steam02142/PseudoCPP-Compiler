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

    if(tokens[current].ttype == Identifier || isLiteral(tokens[current])){
        expression += tokens[current].content;
        current++;
        return true;
    }

    if(tokens[current].ttype == LBrack){
        expression += "(";

        if(tokens[current].ttype != RBrack){
            current++;
            if (!parseBinaryExpr(tokens, current, expression)) { return false; };
            
        } else if (tokens[current].ttype == Add) {
            expression += " " + tokens[current].content + " ";
            current++;
        }
        expression += ")";
        current++;
        return true;
    }
    
    
    
}

bool parseBinaryExpr(token tokens[], int& current, string& expression)
{
   if(!parseExpr(tokens, current, expression)) { return false; }
   
   expression += " " + tokens[current].content + " ";
   current++;
   
   if(!parseExpr(tokens, current, expression)) { return false; }
   return true;
}