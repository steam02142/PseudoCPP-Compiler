#pragma once
#include <iostream>
#include <string>

using namespace std;

enum SymbolType { invalid = -1, integer, real, text, boolean };

const int MaxEntries = 256;
const int MaxParams = 16;
const int MaxScopes = 128;

// ---- Variable table setup ---- 
struct VariableEntry {
    string name;
    SymbolType type;
    int scope;
};

// ---- Procedure table setup ---- 
struct ProcParam {
    string name;
    SymbolType type;
};

struct ProcedureEntry {
    string name;
    int numParams;
    struct ProcParam parameters[MaxParams];
};


void initializeScope();

bool pushScope();

bool popScope();

void printActiveScopes();

bool insertVariable(string name, SymbolType type);

void printVariables();

bool variableExists(string name);

bool variableInScope(string name, int scope);

SymbolType getVariableType(string name);

// procedures
bool insertProcedure(string name);

bool insertProcedureParam(string name, int type);

bool procedureExists(string name);

void printProcedures();