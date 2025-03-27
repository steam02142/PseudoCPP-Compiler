#include "../include/parsing.h"
#include "../include/context_checker.h"

int indent = 0;
int currentLine = 0;

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

ostream& codeEndl(ostream& os)
{
    os << endl;
    currentLine++;
    return os;
}

void printIndent()
{
   cout << string(indent * 3, ' ');
}

string getIndent()
{
    return string(indent * 3, ' ');
}

bool isLiteral(token token)
{
    if(token.ttype == IntLit || token.ttype == RealLit || token.ttype == TextLit || token.ttype == BoolLit)
    {
        return true;
    }
    return false;
}

bool isDataType(token token)
{
    if(token.ttype == Integer || token.ttype == Real || token.ttype == Text || token.ttype == Boolean)
    {
        return true;
    }
    return false;
}

string dataTypeToString(token token)
{
    switch(token.ttype) {
        case Integer: return "long";
        case Real: return "double";
        case Text: return "string";
        case Boolean: return "bool";
        default: return "Invalid";
    }
}

SymbolType getLiteralType(unsigned int ttype)
{
    if(ttype == IntLit){ return integer; } 
    else if (ttype == RealLit) { return real; }
    else if(ttype == TextLit) { return text; }
    else { return boolean; }
}

string typeToString(SymbolType type)
{
    switch(type) {
        case integer: return "long";
        case real: return "double";
        case text: return "string";
        case boolean: return "bool";
        default: return "Invalid";
    }
}

bool parseGlobals(token tokens[], int& current)
{
    if(tokens[current].ttype == Set) {
        current++;
        if (!parseGlobalVariable(tokens, current)) { 
            cerr << "Error: Issue parsing global variables" << endl;
            return false; 
        }
    } else if(tokens[current].ttype == Function) {
        current++;
        //parse
        if (!parseProcedure(tokens, current)) { 
            cerr << "Error: Issue parsing procedure declaration" << endl;
            return false; 
        }
    }

    return true;
}


bool parseGlobalVariable(token tokens[], int& current)
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
        current++;
    } else {
        cerr << "Error: expected assignment operator 'TO', but got " << tokens[current].content << endl;
        return false;
    }

    expressionType = parseExpr(tokens, current, expression);
    if(expressionType == invalid) {
        cerr << "Error: Error occured while parsing expression, made it so far as: " << endl;
        cerr << "\t " << expression << endl;
        return false;
    }
    string type = typeToString(expressionType); 

    if(variableExists(identifier)) {
        cout << identifier << " = " << expression << endl;
        
        SymbolType varType = getVariableType(identifier);
        if(calculateType(varType, expressionType) == invalid) {
            cerr << "Error: incompatible type assignment" << endl;
            return false;
        }
        
    } else {
        cout << type << " " << identifier << " = " << expression << endl;
        insertVariable(identifier, expressionType);
    }
    
    parseGlobals(tokens, current);
    return true;
}

// proc looks like
// FUNCTION funcname ( test: int , var : float)
bool parseProcedure(token tokens[], int& current)
{
    string procedureName;
    
    if(tokens[current].ttype == Identifier) {
        procedureName = tokens[current].content;
        // Add the procedure and enter a new scope for paramlist
        if(!insertProcedure(procedureName)) { return false; }
        if(!pushScope()) { return false; }
        current++;
    } else {
        cerr << "Error: expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    // parse paramlist
    string paramlist;
    parseParamList(tokens, current, paramlist); 

    ostringstream functionStream;
    functionStream << procedureName << paramlist << '\n';
    

    // parse body
    parseBody(tokens, current, functionStream);
    cout << functionStream.str();

    // from body, get return type and output procedure

    return true;

}

bool parseParamList(token tokens[], int& current, string& paramlist)
{
    // Empty parameter list
    if(tokens[current].ttype == LRBrack) {
        current++;
        return true;
    }

    if(tokens[current].ttype == LBrack) {
        paramlist = tokens[current].content;
        current++;
    } else {
        cerr << "Error: expected left bracket, but got " << tokens[current].content << endl;
        return false;
    }

    string paramName;
    while (tokens[current].ttype != RBrack) {
        if(tokens[current].ttype == Parameter) {
            paramName = tokens[current].content;
            current++;
        } else {
            cerr << "Error: expected left bracket, but got " << tokens[current].content << endl;
            return false;
        }

        if(isDataType(tokens[current])) {
            paramlist += dataTypeToString(tokens[current]) + " " + paramName; 
            insertProcedureParam(paramName, tokenTypeToSymbolType(tokens[current].ttype));
            current++;
        } else {
            cerr << "Error: expected data type, but got " << tokens[current].content << endl;
            return false;
        }

        if(tokens[current].ttype == Comma) {
            paramlist += tokens[current].content + " ";
            current++;
        } else if(tokens[current].ttype == RBrack) {
            break;
        } else {
            cerr << "Error: expected comma, but got " << tokens[current].content << endl;
            return false;
        }
    }

    if(tokens[current].ttype == RBrack) {
        paramlist += tokens[current].content;
        current++;
    } else {
        cerr << "Error: expected left bracket, but got " << tokens[current].content << endl;
        return false;
    }

    return true;
}

SymbolType parseBody(token tokens[], int& current, ostringstream& functionStream)
{
    indent++;

    while(tokens[current].ttype != EndFunction){
        functionStream << getIndent();
        switch(tokens[current].ttype){
            case Set:
                current++;
                if (!parseVariable(tokens, current, functionStream)) { return invalid; }
                break;
            case Read:
                break;
            case Print:
                break;
            case Call:
                break;
            // case If:
            //     break;
            // case ForLoop:
            //     break;

            default:
                return invalid;
        }
    }
    indent--;
}


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

bool parseVariable(token tokens[], int& current, ostringstream& functionStream)
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
        current++;
    } else {
        cerr << "Error: expected assignment operator 'TO', but got " << tokens[current].content << endl;
        return false;
    }

    expressionType = parseExpr(tokens, current, expression);
    if(expressionType == invalid) {
        cerr << "Error: Error occured while parsing expression, made it so far as: " << endl;
        cerr << "\t " << expression << endl;
        return false;
    }
    string type = typeToString(expressionType); 

    if(variableExists(identifier)) {
        //cout << identifier << " = " << expression << endl;
        functionStream << identifier << " = " << expression << '\n';

        SymbolType varType = getVariableType(identifier);
        if(calculateType(varType, expressionType) == invalid) {
            cerr << "Error: incompatible type assignment" << endl;
            return false;
        }
        
    } else {
        //cout << type << " " << identifier << " = " << expression << endl;
        functionStream << type << " " << identifier << " = " << expression << '\n';
        insertVariable(identifier, expressionType);
    }
    
    return true;

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

bool isOperator(token token)
{
    if (token.ttype == Add || token.ttype == Sub || token.ttype == Mul || token.ttype == Div || token.ttype == Rem){
        return true;
    }
    cerr << "Error: expected operator but got '" << token.content << "' " << endl;  
    return false;
}

SymbolType parseBinaryExpr(token tokens[], int& current, string& expression)
{
    // Parse left hand side
    SymbolType expr1Type = parseExpr(tokens, current, expression);
    if(expr1Type == invalid) { return invalid; }
   
    // add the operator
    if(!isOperator(tokens[current])) { return invalid; }
    expression += " " + tokens[current].content + " ";
    current++;

    // Parse right hand side 
    SymbolType expr2Type = parseExpr(tokens, current, expression);
    if(expr2Type == invalid) { return invalid; }

    // Check for compatability of LHS and RHS
    SymbolType combinedType = calculateType(expr1Type, expr2Type);
    if(combinedType == invalid) {
        cout << "Error: type mismatch in expression" << endl;
    }

    return combinedType;
}