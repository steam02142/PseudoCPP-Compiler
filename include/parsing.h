#pragma once

#include <iostream>
#include "tokenizing.h"
using namespace std;

bool parse(token tokens[], int numTokens);

bool parseGlobals(token tokens[], int& current);

bool parseGlobalVars(token tokens[], int& current);

bool parseExpr(token tokens[], int& current, string& expression);

bool parseBinaryExpr(token tokens[], int& current, string& expression);