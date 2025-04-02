#include "../include/parsing.h"
#include "../include/context_checker.h"

int indent = 0;

//ofstream OutputProgram("output.cpp");
ostream& OutputProgram = cout;

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

    OutputProgram << "#include <vector>" << endl
                << "#include <string>" << endl
                << "#include <iostream>" << endl << endl
                << "using namespace std;" << endl;

    if (!parseGlobals(tokens, pos, numTokens)) {
        cerr << "Error parsing globals" << endl;
    }

    if (!parseMain(tokens, pos)) {
        cerr << "Error parsing main" << endl;
    }


    return true;
}

void errorMessage(token tokens)
{
    cerr << endl << "-----------------------------" << endl;
    cerr << "Error (line " << tokens.line << ", col " << tokens.column << "): ";
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
    else if (ttype == Boolean) { return boolean; }
    else if(ttype == TextLit) { return text; }
    else { return real; }
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

bool parseGlobals(token tokens[], int& current, int size)
{
    if (current >= size) { 
        cerr << "Error: Main function not defined" << endl;
        return false; 
    }
    if(tokens[current].ttype == Set) {
        current++;
        if (!parseGlobalVariable(tokens, current, size)) { 
            cerr << "Error: Issue parsing global variables" << endl;
            return false; 
        }
    } else if(tokens[current].ttype == Function) {
        current++;
        //parse
        if (!parseProcedure(tokens, current, size)) { 
            cerr << "Error: Issue parsing procedure declaration" << endl;
            return false; 
        }
    }

    return true;
}


bool parseGlobalVariable(token tokens[], int& current, int size)
{
    string identifier;
    string expression;
    SymbolType expressionType;
    if(tokens[current].ttype == Identifier) {
        identifier = tokens[current].content;
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    if(tokens[current].ttype == To) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " expected assignment operator 'TO', but got " << tokens[current].content << endl;
        return false;
    }

    expressionType = parseExpr(tokens, current, expression);
    if(expressionType == invalid) {
        cerr << "Error: Error occured while parsing expression" << endl;
        return false;
    }
    string type = typeToString(expressionType); 

    if(variableExists(identifier)) {
        OutputProgram << identifier << " = " << expression << ";" << endl;
        
        SymbolType varType = getVariableType(identifier);
        if(calculateType(varType, expressionType) == invalid) {
            errorMessage(tokens[current]);
            cerr << " incompatible type assignment" << endl;
            return false;
        }
        
    } else {
        OutputProgram << type << " " << identifier << " = " << expression << ";" << endl;
        insertVariable(identifier, expressionType);
    }
    
    parseGlobals(tokens, current, size);
    return true;
}

bool parseMain(token tokens[], int& current)
{
    if(tokens[current].ttype == Main) {
        OutputProgram << "int main() {" << endl;
        if(!pushScope()) { return false; }
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " expected main function, but got " << tokens[current].content << endl;
        return false;
    }

    // parse body
    SymbolType returnType = invalid;
    while(tokens[current].ttype != EndMain){
        if (!parseBody(tokens, current, returnType)) {
            cerr << "Error: Issue parsing procedure body" << endl;
            return false;
        }
    }

    OutputProgram << "}" << endl;
    if(!popScope()) { return false; }
    current++;

    return true;
}

// proc looks like
// FUNCTION funcname ( test: int , var : float)
bool parseProcedure(token tokens[], int& current, int size)
{
    string procedureName;
    string procedureType;
    
    if(tokens[current].ttype == Identifier) {
        procedureName = tokens[current].content;
        // Add the procedure and enter a new scope for paramlist
        if(!insertProcedure(procedureName)) { return false; }
        if(!pushScope()) { return false; }
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    // parse paramlist
    string paramlist;
    parseParamList(tokens, current, paramlist); 

    if(tokens[current].ttype == TypeArrow) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " expected '->', but got " << tokens[current].content << endl;
        return false;
    }

    if(isDataType(tokens[current])) {
        procedureType = dataTypeToString(tokens[current]);
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << " datatype but got: " << tokens[current].content << endl;
        return false;
    } 

    OutputProgram << endl << procedureType << " " << procedureName << paramlist << " {" << endl;
    

    // parse body
    SymbolType returnType = invalid;
    while(tokens[current].ttype != EndFunction){
        if (!parseBody(tokens, current, returnType)) {
            cerr << "Error: Issue parsing procedure body" << endl;
            return false;
        }
    }

    if (procedureType != typeToString(returnType)) {
        errorMessage(tokens[current]);
        cerr << "Return type doesn't match procedure return type" << endl;
        return false;
    }
    

    // from body, get return type and output procedure
    current++;
    OutputProgram << "}" << endl << endl;
    if(!popScope()) { return false; }

    parseGlobals(tokens, current, size);

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
        errorMessage(tokens[current]);
        cerr << "expected left bracket, but got " << tokens[current].content << endl;
        return false;
    }

    string paramName;
    while (tokens[current].ttype != RBrack) {
        if(tokens[current].ttype == Parameter) {
            paramName = tokens[current].content;
            current++;
        } else {
            errorMessage(tokens[current]);
            cerr << "expected left bracket, but got " << tokens[current].content << endl;
            return false;
        }

        if(isDataType(tokens[current])) {
            token dataTypeToken = tokens[current];
            current++;
            // this is an array 
            if(tokens[current].ttype == SquareLBrack) {
                current++;
                if(tokens[current].ttype != SquareRBrack) {
                    errorMessage(tokens[current]); 
                    cerr << "expected closing bracket for array parameter" << endl;
                    return false;
                }
                current++;

                paramlist += "vector<" + dataTypeToString(dataTypeToken) + "> " + paramName; 
                // add as procedure param for type checking during call later
                insertProcedureParam(paramName, tokenTypeToSymbolType(dataTypeToken.ttype), true);
                // add as variable so we can check it exists before use
                insertVariable(paramName, tokenTypeToSymbolType(dataTypeToken.ttype), true);
                

            }
            else {
                paramlist += dataTypeToString(dataTypeToken) + " " + paramName; 
                // add as procedure param for type checking during call later
                insertProcedureParam(paramName, tokenTypeToSymbolType(dataTypeToken.ttype));
                // add as variable so we can check it exists before use
                insertVariable(paramName, tokenTypeToSymbolType(dataTypeToken.ttype));
                
            }
        } else {
            errorMessage(tokens[current]); 
            cerr << "expected data type, but got " << tokens[current].content << endl;
            return false;
        }

        if(tokens[current].ttype == Comma) {
            paramlist += tokens[current].content + " ";
            current++;
        } else if(tokens[current].ttype == RBrack) {
            break;
        } else {
            errorMessage(tokens[current]);
            cerr << "expected comma, but got " << tokens[current].content << endl;
            return false;
        }
    }

    if(tokens[current].ttype == RBrack) {
        paramlist += tokens[current].content;
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected left bracket, but got " << tokens[current].content << endl;
        return false;
    }

    return true;
}

bool parseBody(token tokens[], int& current, SymbolType& returnType)
{
    indent++;


        OutputProgram << getIndent();
        switch(tokens[current].ttype){
            case Set:
                current++;
                if (!parseVariable(tokens, current)) { return false; }
                break;
            case Read:
                current++;
                if (!parseRead(tokens, current)) { return false; }
                break;
            case Print:
                current++;
                if (!parsePrint(tokens, current)) { return false; }
                break;
            case Call:
                current++;
                if (!parseCall(tokens, current)) { return false; }
                break;
            case If:
                current++;
                if (!parseIf(tokens, current)) { 
                    cerr << "Error: Issue parsing IF statement" << endl;
                    return false; 
                }
                break;
            case ForLoop:
                current++;
                if (!parseForLoop(tokens, current)) { 
                    cerr << "Error: Issue parsing IF statement" << endl;
                    return false; 
                }
                break;
            case Return:
                current++;
                returnType = parseReturn(tokens, current);
                if(returnType == invalid) { 
                    cerr << "Error: Issue parsing RETURN statement" << endl;
                    return false; 
                }
                break;

            default:
                errorMessage(tokens[current]);
                cerr << "Unexpected token: " << tokens[current].content 
                     << ". This token is not valid in this context." << endl;;
                return false;
        }
    
    indent--;

    return true;
}

bool parseRead(token tokens[], int& current)
{
    if(tokens[current].ttype == Identifier) {
        if(!variableExists(tokens[current].content)) { 
            errorMessage(tokens[current]);
            cerr << "variable not declared before attempting to read into it " << endl;
            return false; 
        }
        OutputProgram << "cin >> " << tokens[current].content << ";" << endl;
        current++;
        return true;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected identifier, but got " << tokens[current].content << endl;
        return false;
    }
}

bool parsePrint(token tokens[], int& current)
{
    string expression;
    if(parseExpr(tokens, current, expression) == invalid){
        cerr << "Error: Error parsing print" << endl;
        return false;
    }

    OutputProgram << "cout << " << expression << ";" << endl;

    return true;
}

bool parseCall(token tokens[], int& current)
{
    string procedureName = tokens[current].content;
    // check it is a valid procedure
    if(!procedureExists(tokens[current].content)) {
        errorMessage(tokens[current]);
        cerr << "procedure not defined before call" << endl;
        return false;
    }
    OutputProgram << procedureName << "(";
    current++;

    if(tokens[current].ttype == With) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected 'WITH', but got " << tokens[current].content << endl; 
        return false;
    } 
    
    int index = 0; 
    // parse the passed parameters
    while(tokens[current].ttype == Identifier) 
    {
        //cout << "Deal with call later" << endl;
        string expression;
        SymbolType expressionType = parseExpr(tokens, current, expression);
        SymbolType procedureParamType = getProcedureParamType(procedureName, index);

        if(expressionType != procedureParamType) {
            cerr << "Error: Type mismatch in procedure call" << endl;
            return false;
        }

        OutputProgram << typeToString(expressionType) << " " << expression;
        
        index++;
        // Move to next identifer if there are multiple
        if(tokens[current].ttype == Comma) { 
            OutputProgram << ", ";
            current++; 
        }
    }    

    OutputProgram << ");" << endl;
    return true;
}

bool parseForLoop(token tokens[], int& current)
{
    string identifier;
    pushScope();
    OutputProgram << "for (";
    // already parsed 'FOR'
    if(tokens[current].ttype == Identifier) {
        identifier = tokens[current].content;
        if(variableExists(identifier)) {
            errorMessage(tokens[current]);
            cerr << "cannot reuse variables in for loop, already defined: " << tokens[current].content << endl;
            return false;
        }

        insertVariable(identifier, tokenTypeToSymbolType(Integer));

        OutputProgram << "int " << identifier << " = "; 
        
        current++;
         
    } else {
        errorMessage(tokens[current]);
        cerr << "expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    if(tokens[current].ttype == From) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected 'FROM', but got " << tokens[current].content << endl; 
        return false;
    }  

    // number/var
    string expression;
    SymbolType expressionType = parseExpr(tokens, current, expression);
    if(expressionType != integer){
        cerr << "Error: Error parsing for loop, requires integer initialization" << endl;
        return false;
    } 
    OutputProgram << expression << "; ";

    //TO
    if(tokens[current].ttype == To) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected 'TO', but got " << tokens[current].content << endl; 
        return false;
    }  

    // number/var
    expression = "";
    expressionType = parseExpr(tokens, current, expression);
    if(expressionType != integer){
        cerr << "Error: Error parsing for loop, requires integer bound" << endl;
        return false;
    } 
    OutputProgram << identifier << " <= " << expression << "; " << identifier << "++) {" << endl;

    indent++;
    // while not EndForLoop -> body
    SymbolType returnType = invalid;
    while(tokens[current].ttype != EndForLoop){
        if (!parseBody(tokens, current, returnType)) {
            cerr << "Error: Issue parsing for loop body" << endl;
            return false;
        }
    }

    current++;

    // close loop
    popScope();
    indent--;

    OutputProgram << getIndent() << "}" << endl << endl; 
       
    
    

    return true;
}

SymbolType parseReturn(token tokens[], int& current)
{
    string expression;
    SymbolType expressionType = parseExpr(tokens, current, expression);
    if(expressionType == invalid){
        cerr << "Error: Error parsing print" << endl;
        return invalid;
    }

    OutputProgram << "return " << expression << ";" << endl;

    return expressionType;
}

bool parseArray(token tokens[], int& current, string& arrayIndex)
{
    // ensure an index is passed
    if(tokens[current].ttype == IntLit){
        // Check it is within bounds at some point -----------------------------------------------------------------------------------------
        arrayIndex += "[" + tokens[current].content + "]";
        current++;
    } else if (tokens[current].ttype == Identifier) {
        if(!variableExists(tokens[current].content)) { 
            errorMessage(tokens[current]);
            cerr << "variable " << tokens[current].content << " not defined before use as array index" << endl; 
            return false; 
        }

        if(getVariableType(tokens[current].content) != integer){
            errorMessage(tokens[current]);
            cerr << "variable " << tokens[current].content << " is being used as an index but is not an integer" << endl; 
            return false; 
        }

        arrayIndex += "[" + tokens[current].content + "]";
        current++;
    }
    else {
        errorMessage(tokens[current]);
        cerr << "array expects index" << endl;
        return false;
    }

    // check closing bracket present
    if(tokens[current].ttype == SquareRBrack) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "array expects closing bracket" << endl;
        return false;
    }
    return true;
}

// returns the type that the expression evaluates to
SymbolType parseExpr(token tokens[], int& current, string& expression)
{
    SymbolType type = invalid;

    if(tokens[current].ttype == Identifier){
        // check ident exists and get type
        expression += tokens[current].content;
        if(!variableExists(tokens[current].content)) { 
            errorMessage(tokens[current]);
            cerr << "variable " << tokens[current].content << " not defined before use" << endl; 
            return invalid; 
        }

        type = getVariableType(tokens[current].content);
        current++;
    

        // handle arrays
        if(tokens[current].ttype == SquareLBrack) {
            current++;
            if (!parseArray(tokens, current, expression)) { return invalid; }
        } 


        
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
            errorMessage(tokens[current]);
            cerr << "missing closing bracket " << endl;
            return invalid;
        } 
        
        expression += ")";
        current++;
        return type;
    } 
    // This is an array
    else if(tokens[current].ttype == SquareLBrack) {
        expression += "{";
        current++;
        // use the first value in the array to determine the type
        type = getLiteralType(tokens[current].ttype);
        
        while(tokens[current].ttype != SquareRBrack) {
            // get value
            expression += tokens[current].content;
            // put comma if we aren't on the last token
            current++;
            if(tokens[current].ttype != SquareRBrack) {
                expression += tokens[current].content + " ";
                current++;
            }
            
        }
        
        expression += "}";
        current++;
        return type;
    }
    
    else {
        // Reached end of input without bracket
        errorMessage(tokens[current]);
        cerr << "missing closing bracket " << endl;
        return invalid;
    }
    
}

bool parseVariable(token tokens[], int& current)
{
    string identifier;
    string arrayIndex;
    string expression;
    SymbolType expressionType;
    bool isArray = false;
    if(tokens[current].ttype == Identifier) {
        identifier = tokens[current].content;
        current++;
        if(tokens[current].ttype == SquareLBrack) {
            current++;
            if (!parseArray(tokens, current, arrayIndex)) { return false; }
            isArray = true;
        } 
    } else {
        errorMessage(tokens[current]);
        cerr << "expected identifier, but got " << tokens[current].content << endl;
        return false;
    }

    if(tokens[current].ttype == To) {
        current++;
    } else {
        errorMessage(tokens[current]);
        cerr << "expected assignment operator 'TO', but got " << tokens[current].content << endl;
        return false;
    }

    // Check if we are defining an array before processing it and getting the type
    if(tokens[current].ttype == SquareLBrack) { isArray = true; }

    expressionType = parseExpr(tokens, current, expression);
    if(expressionType == invalid) {
        cerr << "Error occured while parsing expression" << endl;
        return false;
    }
    string type = typeToString(expressionType); 

    if(variableExists(identifier)) {
        //cout << identifier << " = " << expression << endl;
        if(isArray) {
            OutputProgram << identifier << arrayIndex << " = " << expression << ";" << endl;
        } else {
            OutputProgram << identifier << " = " << expression << ";" << endl;
        }
        

        SymbolType varType = getVariableType(identifier);
        if(calculateType(varType, expressionType) == invalid) {
            errorMessage(tokens[current]);
            cerr << "incompatible type assignment" << endl;
            return false;
        }
        
    } else {
        //cout << type << " " << identifier << " = " << expression << endl;

        if(isArray) {
            OutputProgram << "vector<" << type << "> " << identifier << " = " << expression << ";" << endl;
        } else {
            OutputProgram << type << " " << identifier << " = " << expression << ";" << endl;
        }

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
    errorMessage(token);
    cerr << "expected operator but got '" << token.content << "' " << endl;  
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
        errorMessage(tokens[current]);
        cerr << "type mismatch in expression" << endl;
    }

    return combinedType;
}

bool isComparisonOp(unsigned int ttype) {
    return ttype == Lt || ttype == Le || ttype == Gt ||
           ttype == Ge || ttype == Eq || ttype == Ne;
}

bool parseIf(token tokens[], int& current) {
    
    OutputProgram << "if (";
    
    // Parse condition
    if (!parseCondition(tokens, current)) {
        return false;
    }
    
    OutputProgram << ") {" << endl;
    // Entering if statemetn scope
    if(!pushScope()) { return false; }
    
    indent++;
    
    // Parse body
    SymbolType dummyReturn; // just used for function call
    while (tokens[current].ttype != Else && tokens[current].ttype != EndIf) {
        if (!parseBody(tokens, current, dummyReturn)) {
            errorMessage(tokens[current]);
            cerr << "Error in IF body" << endl;
            return false;
        }
    }
    
    indent--;
    // exiting if statement scope
    if(!popScope()) { return false; }
    
    OutputProgram << getIndent() << "}";
    
    // Handle ELSE
    if ( tokens[current].ttype == Else) {
        current++;
        OutputProgram << " else {" << endl;
        indent++;
        // Entering else statemetn scope
        if(!pushScope()) { return false; }
        
        while (tokens[current].ttype != EndIf) {
            if (!parseBody(tokens, current, dummyReturn)) {
                errorMessage(tokens[current]);
                cerr << "Error in ELSE body" << endl;
                return false;
            }
        }
        
        indent--;
        // exiting else statement scope
        if(!popScope()) { return false; }
        OutputProgram << getIndent() << "}";
    }
    
    // Ensure IF statement closed properly
    if ( tokens[current].ttype != EndIf) {
        errorMessage(tokens[current]);
        cerr << "Expected ENDIF" << endl;
        return false;
    }
    current++;
    
    OutputProgram << endl;
    return true;
}

// Parses condition with the following precedence from highest to lowest: Brackets, NOT, comparison op, AND, OR 
bool parseCondition(token tokens[], int& current) {
    return parseOrCondition(tokens, current);
}

bool parseOrCondition(token tokens[], int& current) {
    if (!parseAndCondition(tokens, current)) {
        return false;
    }

    while (tokens[current].ttype == Or) {
        OutputProgram << " " << tokens[current].content << " ";
        current++;
        if (!parseAndCondition(tokens, current)) {
            return false;
        }
    }
    return true;
}

bool parseAndCondition(token tokens[], int& current) {
    if (!parsePrimaryCondition(tokens, current)) {
        return false;
    }

    while (tokens[current].ttype == And) {
        OutputProgram << " " << tokens[current].content << " ";
        current++;
        if (!parsePrimaryCondition(tokens, current)) {
            return false;
        }
    }
    return true;
}

bool parsePrimaryCondition(token tokens[], int& current) {
    // Handle NOT
    if (tokens[current].ttype == Not) {
        OutputProgram << "!";
        current++;
    }

    // Handle parentheses
    if (tokens[current].ttype == LBrack) {
        OutputProgram << "(";
        current++;
        if (!parseOrCondition(tokens, current)) {
            return false;
        }
        if (tokens[current].ttype != RBrack) {
            errorMessage(tokens[current]);
            cerr << "Expected ')'" << endl;
            return false;
        }
        OutputProgram << ")";
        current++;
        return true;
    }

    // Handle basic comparison
    string leftExpr, rightExpr;
    if (parseExpr(tokens, current, leftExpr) == invalid) {
        cerr << "Error: Issue in left expression" << endl;
        return false;
    }
    OutputProgram << leftExpr;

    if (!isComparisonOp(tokens[current].ttype)) {
        errorMessage(tokens[current]);
        cerr << "Error: Expected comparison operator" << endl;
        return false;
    }
    OutputProgram << " " << tokens[current].content << " ";
    current++;

    if (parseExpr(tokens, current, rightExpr) == invalid) {
        cerr << "Error: Issue in right expression" << endl;
        return false;
    }
    OutputProgram << rightExpr;

    return true;
}

