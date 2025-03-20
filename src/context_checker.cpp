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


void initializeScope()
{
   scopes[0] = 0;
   currentScope = 0;
   // number of scopes active, just global is active to start
   activeScopes = 1;
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
}

void printActiveScopes()
{
    for(int i = 0; i < activeScopes; i++) {
        cout << scopes[i] << " ";
    }
    cout << endl << endl;
}