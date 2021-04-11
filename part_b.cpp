#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <stack>
#include "tokens.hpp"

using std::stack;
using std::cout;
using std::endl;

extern int yylex();

void printCharByIndent(char c, int indent) {
    for (int i = 0; i < indent; ++i) {
        cout << '\t';
    }
    cout << c << endl;
}

int main() {
    int token;
    int indent = 0;
    stack<char> braces;
    while (token = yylex()) {
        if (token == ERRORCHAR) {
            cout << "Error " << yytext << endl;
            exit(0);
        }
        if (token != LPAREN && token != RPAREN && token != LBRACE && token != RBRACE) {
            cout << "Error: " << TOKEN_NAMES[token] << endl;
            exit(0);
        }
        if (token == LPAREN) {
            braces.push('(');
            printCharByIndent('(', indent);
            indent++;
            continue;
        }
        if (token == LBRACE) {
            braces.push('{');
            printCharByIndent('{', indent);
            indent++;
            continue;
        }
        if (braces.empty()) {
            cout << "Error: Bad Expression" << endl;
            exit(0);
        }
        char top = braces.top();
        braces.pop();
        if (token == RPAREN && top != '(') {
            cout << "Error: Bad Expression" << endl;
            exit(0);
        }
        if (token == RBRACE && top != '{') {
            cout << "Error: Bad Expression" << endl;
            exit(0);
        }
        if (token == RPAREN) {
            indent--;
            printCharByIndent(')', indent);
            continue;
        } else {
            indent--;
            printCharByIndent('}', indent);
            continue;
        }
    }
    if (!braces.empty()) {
        cout << "Error: Bad Expression" << endl;
        exit(0);
    }
    return 0;
}