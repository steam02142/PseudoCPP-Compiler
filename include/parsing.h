#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include "tokenizing.h"
#include "../include/context_checker.h"
using namespace std;

bool parse(token tokens[], int numTokens);

bool parseGlobals(token tokens[], int& current);

bool parseGlobalVariable(token tokens[], int& current);

bool parseProcedure(token tokens[], int& current);

bool parseParamList(token tokens[], int& current, string& paramlist);

SymbolType parseExpr(token tokens[], int& current, string& expression);

SymbolType parseBinaryExpr(token tokens[], int& current, string& expression);

SymbolType calculateType(SymbolType type1, SymbolType type2);

SymbolType parseBody(token tokens[], int& current, ostringstream& functionStream);

bool parseVariable(token tokens[], int& current, ostringstream& functionStream);