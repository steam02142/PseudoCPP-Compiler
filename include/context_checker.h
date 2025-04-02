#pragma once
#include <iostream>
#include <string>
#include "tokenizing.h"

using namespace std;

enum SymbolType { invalid = -1, integer = Integer, real = Real, text = Text, boolean = Boolean, array };

const int MaxEntries = 256;
const int MaxParams = 16;
const int MaxScopes = 128;

// ---- Variable table setup ---- 
struct VariableEntry {
    string name;
    SymbolType type;
    int scope;
    bool isArray;
};

// ---- Procedure table setup ---- 
struct ProcParam {
    string name;
    SymbolType type;
    bool isArray;
};

struct ProcedureEntry {
    string name;
    int numParams;
    struct ProcParam parameters[MaxParams];
};

enum SymbolType tokenTypeToSymbolType(unsigned int type);

void initializeScope();

bool pushScope();

bool popScope();

void printActiveScopes();

bool insertVariable(string name, SymbolType type, bool isArray = false);

void printVariables();

bool variableExists(string name);

bool variableInScope(string name, int scope);

SymbolType getVariableType(string name);

// procedures
bool insertProcedure(string name);

bool insertProcedureParam(string name, SymbolType type, bool isArray = false);

bool procedureExists(string name);

SymbolType getProcedureParamType(string procedureName, int paramPosition);

void printProcedures();