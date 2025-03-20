#pragma once
#include <iostream>
#include <string>

using namespace std;

enum SymbolType { invalid = -1, integer, realNum, stringLit };

const int MaxEntries = 256;
const int MaxParams = 16;
const int MaxScopes = 128;

// ---- Variable table setup ---- 
struct VariableEntry {
    string name;
    int type;
    int scope;
};

// ---- Procedure table setup ---- 
struct ProcParam {
    string name;
    int type;
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