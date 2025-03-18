#pragma once

#include <iostream>
#include <string>
using namespace std;

enum TokenType {
    Invalid = -1, Function, Read, Print, Compute, Set, Call, Increment, Decrement, 
    Add, Sub, Mul, Div, EndFunction 
};

// 
struct token {
    unsigned int ttype;
    string content;
    int pos;
};

const int MaxTokens = 1024;

/** @brief Read a program from standard input and store token information in an array.
 *  @return The number of tokens successfully read and stored.
*/
int tokenize(token tokens[]);

void storeToken(token& token, unsigned int ttype, string content, int& pos);

bool hanldeKeyword(string input, token& token, int& pos);

void printTokens(token tokens[], int size);

string ttypeTostr(unsigned int ttype);

