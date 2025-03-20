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
    if(activeScopes > MaxScopes){
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
bool insertVariable(string name, int type, int scope)
{
    if(variableInScope(name, scope)) { 
        cerr << "Error: variable redeclaration" << endl;
        return false; 
    }

    VariableTable[numVariables].name = name;
    VariableTable[numVariables].type = type;
    VariableTable[numVariables].scope = scope;

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
    for(int scope = activeScopes - 1; scope >= 0; scope--){
        if(variableInScope(name, scope)){ return true; }
    }
    cerr << "Error: variable '" << name << "' not defined before use" << endl; 
    return false;
}


void printVariables()
{
    for(int i = 0; i < numVariables; i++) {
        cout << VariableTable[i].name << " ";
    }
    cout << endl;
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

bool insertProcedureParam(string name, int type)
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
    ProcedureTable[currentProc].numParams++;

    return true;
}

void printProcedures()
{
    for(int i = 0; i < numProcedures; i++){
        cout << "Procedure: ";
        cout << ProcedureTable[i].name << endl; 
        cout << "Parameters: ";
        for(int j = 0; j < ProcedureTable[i].numParams; j++){
            cout << ProcedureTable[i].parameters[j].name << " ";
        }
        cout << endl << endl;
    }
}