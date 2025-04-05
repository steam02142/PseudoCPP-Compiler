#pragma once

#include <iostream>
#include <string>
#include <regex>
using namespace std;

enum TokenType {
    Invalid = -1, Function, Main, EndMain, Read, Print, Compute, Set, To, Call, With, Increment, Decrement, 
    Add, Sub, Mul, Div, Rem, TextLit, IntLit, RealLit, BoolLit, Identifier, Returning,
    Lt, Gt, Le, Ge, Ne, Eq, And, Or, Not, If, Else, EndIf, ForLoop, EndForLoop, From, TypeArrow, Return,
    Integer, Real, Boolean, Text, Parameter, Comma, LBrack, RBrack, LRBrack, SquareRBrack, SquareLBrack, EndFunction
};
// Integer is 40

// 
struct token {
    unsigned int ttype;
    string content;
    int pos;
    int line;
    int column;
};

const int MaxTokens = 1024;

/** @brief Read a program from standard input and store token information in an array.
 *  @return The number of tokens successfully read and stored.
*/
int tokenize(token tokens[]);

void processToken(string& currentToken, token tokens[], int& pos, int currentLine, int currentColumn, bool& invalidTokenDetected);

void storeToken(token tokens[], unsigned int ttype, string input, int& pos, int line, int column);

bool hanldeKeyword(string input, token tokens[], int& pos, int line, int column);

void printTokens(token tokens[], int size);

string ttypeTostr(unsigned int ttype);

