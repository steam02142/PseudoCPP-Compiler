#include "../include/context_checker.h"

// ---- Variable table setup ---- 
struct VariableEntry VariableTable[MaxEntries];
int numVariables = 0;

// ---- Procedure table setup ---- 
struct ProcedureEntry ProcedureTable[MaxEntries];
int numProcedures = 0;

// ---- Scope stack setup ---- 
int scopes[MaxScopes];
int activeScopes; 
int currentScope;
int nextScope;

// ---- Scope functions ----
void initializeScope()
{
   scopes[0] = 0;
   currentScope = 0;
   activeScopes = 1; // number of scopes active, just global is active to start
   nextScope = 1;
}

bool pushScope()
{
    if(activeScopes >= MaxScopes){
        return false;
    }

    scopes[activeScopes] = nextScope;
    currentScope = nextScope;
    nextScope++;
    activeScopes++;
    return true;
}

bool popScope()
{
    if(activeScopes <= 1){
        return false;
    }

    activeScopes--;
    currentScope = scopes[activeScopes];
    return true;
}

void printActiveScopes()
{
    cout << "Active Scopes: ";
    for(int i = 0; i < activeScopes; i++) {
        cout << scopes[i] << " ";
    }
    cout << endl << endl;
}

// ---- Variable Stack Functions ----
bool insertVariable(string name, SymbolType type, bool isArray)
{
    if(variableInScope(name, currentScope)) { 
        cerr << "Error: variable redeclaration" << endl;
        return false; 
    }

    VariableTable[numVariables].name = name;
    VariableTable[numVariables].type = type;
    VariableTable[numVariables].scope = currentScope;
    VariableTable[numVariables].isArray = isArray;


    numVariables++;
    return true;
}

bool variableInScope(string name, int scope)
{
    for(int i = 0; i < numVariables; i++){
        if(VariableTable[i].name == name && VariableTable[i].scope == scope) {
            return true;
        }
    }
    return false;
}

bool variableExists(string name)
{
    for(int i = activeScopes; i >= 0; i--){
        int scopeNumberOnStack = scopes[i];
        if(variableInScope(name, scopeNumberOnStack)){ return true; }
    } 
    return false;
}

SymbolType getVariableType(string name)
{
    for(int scope = activeScopes; scope >= 0; scope--){
        int scopeNumberOnStack = scopes[scope];
        for(int i = 0; i < numVariables; i++){
            if(VariableTable[i].name == name && VariableTable[i].scope == scopeNumberOnStack) {
                return VariableTable[i].type;
            }
        }
    }
    cerr << "Error: variable '" << name << "' not defined before use" << endl; 
    return invalid;
}


void printVariables()
{
    for(int i = 0; i < numVariables; i++) {
        cout << VariableTable[i].name << " ";
    }
    cout << endl;
}

enum SymbolType tokenTypeToSymbolType(unsigned int type)
{
    switch(type){
        case Integer: return integer;
        case Real: return real;
        case Text: return text;
        case Boolean: return boolean;
        default: return invalid;
    }
}


// ---- Procedure Stack Functions ----
bool insertProcedure(string name)
{
    if(procedureExists(name)) { 
        cerr << "Error: procedure '" << name << "' has already been defined" << endl;
        return false; 
    }

    ProcedureTable[numProcedures].name = name;

    numProcedures++;
    return true;
}

bool procedureExists(string name)
{
    for(int i = 0; i < numProcedures; i++){
        if(ProcedureTable[i].name == name) { return true; }
    }

    return false;
}

SymbolType getProcedureParamType(string procedureName, int paramPosition)
{
    for(int i = 0; i < numProcedures; i++){
        if(ProcedureTable[i].name == procedureName) {
            if(ProcedureTable[i].numParams > paramPosition) {
                return ProcedureTable[i].parameters[paramPosition].type;
            } else {
                cerr << "Error: out of bounds" << endl;
                return invalid;
            }
        }
    }
    cout << "NO MATCH" << endl;
    return invalid;
}

bool insertProcedureParam(string name, SymbolType type, bool isArray)
{
    int currentProc = numProcedures - 1;
    if(ProcedureTable[currentProc].numParams >= MaxParams){
        cerr << "Error: procedure exceeds maximum number of parameters" << endl;
        return false;
    }

    for(int i = 0; i < ProcedureTable[currentProc].numParams; i++){
        if(ProcedureTable[currentProc].parameters[i].name == name){
            cerr<< "Error: duplicate parameter in procedure declaration" << endl;
            return false;
        }
    }

    int newParam = ProcedureTable[currentProc].numParams;

    ProcedureTable[currentProc].parameters[newParam].name = name;
    ProcedureTable[currentProc].parameters[newParam].type = type;
    ProcedureTable[currentProc].parameters[newParam].isArray = isArray;
    ProcedureTable[currentProc].numParams++;

    return true;
}

void addProcedureReturnType(SymbolType procType)
{
    int currentProc = numProcedures - 1;
    ProcedureTable[currentProc].type = procType;
}

SymbolType getProcedureReturnType(string name)
{
    for(int i = 0; i < numProcedures; i++) {
        if(ProcedureTable[i].name == name) {
            return ProcedureTable[i].type;
        }
    }

    return invalid;
}

int getProcedureNumParams(string name)
{
    for(int i = 0; i < numProcedures; i++) {
        if(ProcedureTable[i].name == name) {
            return ProcedureTable[i].numParams;
        }
    }

    // procedure doesn't exist
    return -1;
}

void printProcedures()
{
    for(int i = 0; i < numProcedures; i++){
        cout << "Procedure: ";
        cout << ProcedureTable[i].name << endl; 
        cout << "Parameters: ";
        for(int j = 0; j < ProcedureTable[i].numParams; j++){
            cout << ProcedureTable[i].parameters[j].name << " (type: " << ProcedureTable[i].parameters[j].type << ") ";
        }
        cout << endl << endl;
    }
}