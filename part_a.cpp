#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include "tokens.hpp"

using std::strlen;
using std::string;
using std::cout;
using std::endl;

extern int yylex();

bool check_legal_hex_pair(const char c1, const char c2) {
    if (c1 >= '0' && c1 <= '7' && (c2 >= '0' && c2 <= '9' || c2 >= 'A' && c2 <= 'F')) {
        return true;
    }
    return false;
}

int replace_escape_sequence(const char c) {
    switch (c) {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case 'r':
            return '\r';
        case '0':
            return '\0';
        case '"':
            return '"';
        case '\\':
            return '\\';
        default:
            return -1;
    }
}

char convert_hex_to_ascii(const char c1, const char c2) {
    int digit2 = -1;
    if (c1 >= '0' && c1 <= '7') {
        int digit1 = c1 - '0';
        if (c2 >= '0' && c2 <= '9') {
            digit2 = c2 - '0';
        } else if (c2 >= 'a' && c2 <= 'f') {
            digit2 = c2 - 'a' + 10;
        } else if (c2 >= 'A' && c2 <= 'F') {
            digit2 = c2 - 'A' + 10;
        } else {
            return -1;
        }
        return digit1 * 16 + digit2;
    }
    return -1;
}

void showToken(const int token) {
    if (token == ERRORCHAR) {
        printf("Error %s\n", yytext);
        exit(0);
    }
    if (token == ERRORSTRING) {
        int strLen = strlen(yytext);
        for (int i = 0; i < strLen; ++i) {
            // Handle newline in the middle of the string, or a '\' as the last character of the string
            if (yytext[i] == '\n' || yytext[i] == '\r' || (yytext[i] == '\\' && i == strLen - 1)) {
                printf("Error unclosed string \n");
                exit(0);
            }
            // Handle an illegal escape character sequence
            if (yytext[i] == '\\') {
                if (yytext[i + 1] != '0' || yytext[i + 1] != '"' || yytext[i + 1] != '\\' || yytext[i + 1] != 't' || yytext[i + 1] != 'x') {
                    printf("Error undefined escape sequence %c\n", yytext[i + 1]);
                    exit(0);
                }
                // Check if the embedded hex character value is legal
                if (yytext[i + 1] == 'x') {
                    if (i + 3 > strLen) {
                        // There is not enough room left in the string for a legal hex value
                        printf("Error undefined escape sequence %c%c\n", yytext[i + 1], yytext[i + 2]);
                        exit(0);
                    } else {
                        if (!check_legal_hex_pair(yytext[i + 2], yytext[i + 3])) {
                            printf("Error undefined escape sequence %c%c%c\n", yytext[i + 1], yytext[i + 2], yytext[i + 3]);
                        }
                    }
                }
            }
        }
    }
    // \x7F
    if (token == COMMENT) {
        printf("%d %s //\n", yylineno, TOKEN_NAMES[token]);
    } else if (token == STRING) {
        int strLen = strlen(yytext);
        string output;
        for (int i = 0; i < strLen - 1; ++i) {
            if (yytext[i] != '\\') {
                // This is a normal character which requires no special handling
                output.push_back(yytext[i]);
            } else {
                // this is part of an escape sequence, so we need to replace the escape sequence with the correct value in the output string
                // figure out what is the escape sequence - n,0,t,r,",\,x
                if (yytext[i + 1] == 'x') {
                    // Need to convert an embedded hex value to it's real ascii character
                    output.push_back(convert_hex_to_ascii(yytext[i + 2], yytext[i + 3]));
                    i += 3;
                } else {
                    // Need to replace the escape sequence with it's real escape sequence meaning
                    int result = replace_escape_sequence(yytext[i + 1]);
                    if (result == -1) {
                        printf("Error undefined escape sequence %c\n", yytext[i + 1]);
                        exit(0);
                    } else {
                        output.push_back(result);
                        i += 1;
                    }
                }
            }
        }
        printf("%d %s ", yylineno, TOKEN_NAMES[token]);
        std::cout << output << std::endl;
    } else {
        // this prints out any other token that wasn't matched before
        printf("%d %s %s\n", yylineno, TOKEN_NAMES[token], yytext);
    }
}

int main() {
    int token;
    while (token = yylex()) {
        showToken(token);
    }
    return 0;
}