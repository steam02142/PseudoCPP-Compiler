#pragma once

#include <iostream>
#include "tokenizing.h"
#include "../include/context_checker.h"
using namespace std;

bool parse(token tokens[], int numTokens);

bool parseGlobals(token tokens[], int& current);

bool parseVariables(token tokens[], int& current);

bool parseProcedure(token tokens[], int& current);

SymbolType parseExpr(token tokens[], int& current, string& expression);

SymbolType parseBinaryExpr(token tokens[], int& current, string& expression);

SymbolType calculateType(SymbolType type1, SymbolType type2);