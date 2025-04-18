#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include "tokenizing.h"
#include "../include/context_checker.h"
using namespace std;

bool parse(token tokens[], int numTokens);

bool parseGlobals(token tokens[], int& current, int size);

bool parseMain(token tokens[], int& current);

bool parseGlobalVariable(token tokens[], int& current, int size);

bool parseProcedure(token tokens[], int& current, int size);

bool parseParamList(token tokens[], int& current, string& paramlist);

SymbolType parseExpr(token tokens[], int& current, string& expression);

SymbolType parseBinaryExpr(token tokens[], int& current, string& expression);

SymbolType calculateType(SymbolType type1, SymbolType type2);

bool parseBody(token tokens[], int& current, SymbolType& returnType);

bool parseVariable(token tokens[], int& current);

bool parseRead(token tokens[], int& current);

bool parsePrint(token tokens[], int& current);

SymbolType parseReturn(token tokens[], int& current);

bool parseForLoop(token tokens[], int& current);

bool parseCall(token tokens[], int& current);



bool parseIf(token tokens[], int& current);
bool parseCondition(token tokens[], int& current);
bool parseOrCondition(token tokens[], int& current);
bool parseAndCondition(token tokens[], int& current);
bool parsePrimaryCondition(token tokens[], int& current);
