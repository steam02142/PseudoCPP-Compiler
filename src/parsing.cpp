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
    } else if(tokens[current].ttype == Set) {
        current++;
        //parse
        cout << "parse procdefs here";
    }

    return true;
}


bool parseGlobalVars(token tokens[], int& current)
{
    string identifier;
    string expression;
    SymbolType expressionType;
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

    expressionType = parseExpr(tokens, current, expression);
    cout << expressionType << " " << expression << endl;


    parseGlobals(tokens, current);
    return true;

}

//( e( 5 + 2) + 3 )
// returns the type that the expression evaluates to
SymbolType parseExpr(token tokens[], int& current, string& expression)
{
    SymbolType type = invalid;

    if(tokens[current].ttype == Identifier){
        // check ident exists and get type
        expression += tokens[current].content;
        if(!variableExists(tokens[current].content)) { return invalid; }
        type = getVariableType(tokens[current].content);
        //return type

        current++;
        return type;
    } else if(isLiteral(tokens[current])){
        // check type and return it 
        expression += tokens[current].content;
        type = getLiteralType(tokens[current].ttype);

        current++;

        // return type
        return type;
    } else if(tokens[current].ttype == LBrack){
        expression += "(";
        current++;

        type = parseBinaryExpr(tokens, current, expression);

        if (type == -1 ) { return invalid; }; 

        // Unexpected token
        if(tokens[current].ttype != RBrack){
            cout << "Missing closing bracket " << endl;
            return invalid;
        } 
        
        expression += ")";
        current++;
        return type;
    } else {
        // Reached end of input without bracket
        cout << "Missing closing bracket " << endl;
        return invalid;
    }
    
}

SymbolType calculateType(SymbolType type1, SymbolType type2)
{
    if(type1 == type2) {
        return type1;
    } else if ((type1 == integer && type2 == real) || (type1 == real && type2 == integer)) {
        return real;
    } else {
        return invalid;
    }
}

SymbolType parseBinaryExpr(token tokens[], int& current, string& expression)
{
    // expr1Type & expr2Type then compare for compatibility (in other function)
    SymbolType expr1Type = parseExpr(tokens, current, expression);
    SymbolType expr2Type;
    SymbolType combinedType;

    if(expr1Type == invalid) { return invalid; }
   
    expression += " " + tokens[current].content + " ";
    current++;

    expr2Type = parseExpr(tokens, current, expression);
   
    if(expr2Type == invalid) { return invalid; }

    combinedType = calculateType(expr1Type, expr2Type);

    if(combinedType == -1) {
        cout << "Error: type mismatch in expression" << endl;
    }

    return combinedType;
}