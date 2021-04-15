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
    if (c1 >= '0' && c1 <= '7' && (c2 >= '0' && c2 <= '9' || c2 >= 'A' && c2 <= 'F' || c2 >= 'a' && c2 <= 'f')) {
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
            return 0;
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
//    cout << "Token num is: " << token << endl;
//    cout << "TEST IS: " << yytext << endl;
    if (token == ERRORCHAR) {
        cout << "Error " << yytext << endl;
        exit(0);
    }
    if (token == UNCLOSEDSTRING) {
//        cout << "HERE" << endl;
        cout << "Error unclosed string" << endl;
        exit(0);
    }

    if (token == COMMENT) {
        cout << yylineno << " " << TOKEN_NAMES[token] << " //" << endl;
    } else if (token == STRING) {
        int strLen = strlen(yytext);
        bool keepWritingOutput = true;
        string undef = "";
        string output;
        for (int i = 0; i < strLen - 1; ++i) {
            if (yytext[i] != '\\') {
                // This is a normal character which requires no special handling
                if (keepWritingOutput) {
                    output.push_back(yytext[i]);
                }
            } else {
                // this is part of an escape sequence, so we need to replace the escape sequence with the correct value in the output string
                // figure out what is the escape sequence - n,0,t,r,",\,x
                if (yytext[i + 1] != '0' && yytext[i + 1] != '"' && yytext[i + 1] != '\\' && yytext[i + 1] != 't' && yytext[i + 1] != 'x' &&
                    yytext[i + 1] != 'n' && yytext[i + 1] != 'r') {
                    undef.append("Error undefined escape sequence ");
                    undef.push_back(yytext[i + 1]);
                    cout << undef << endl;
                    exit(0);
                }
                // Check if the embedded hex character value is legal
                if (yytext[i + 1] == 'x') {
                    if (i + 3 > strLen - 1) {
                        if (strLen - 1 - i == 2) {
                            // There is room for \xA
                            undef.append("Error undefined escape sequence ");
                            if (yytext[i + 2] != '"') {
                                undef.push_back(yytext[i + 1]);
                                undef.push_back(yytext[i + 2]);
                                cout << undef << endl;
                                exit(0);
                            } else {
                                undef.push_back(yytext[i + 1]);
                                cout << undef << endl;
                                exit(0);
                            }
                        } else if (strLen - 1 - i == 1) {
                            // There is room for \x
                            undef.append("Error undefined escape sequence ");
                            undef.push_back(yytext[i + 1]);
                            cout << undef << endl;
                            exit(0);
                        }
                    } else if (!check_legal_hex_pair(yytext[i + 2], yytext[i + 3])) {
                        undef.append("Error undefined escape sequence ");
                        if (yytext[i + 3] == '"') {
                            undef.push_back(yytext[i + 1]);
                            undef.push_back(yytext[i + 2]);
                            cout << undef << endl;
                            exit(0);
                        } else {
                            undef.push_back(yytext[i + 1]);
                            undef.push_back(yytext[i + 2]);
                            undef.push_back(yytext[i + 3]);
                            cout << undef << endl;
                            exit(0);
                        }
                    } else {
                        // Need to convert an embedded hex value to it's real ascii character
                        if (keepWritingOutput) {
                            char res = convert_hex_to_ascii(yytext[i + 2], yytext[i + 3]);
                            if (res != 0) {
                                output.push_back(res);
                            }
                            i += 3;
                        }
                    }
                } else {
                    // Need to replace the escape sequence with it's real escape sequence meaning
                    int result = replace_escape_sequence(yytext[i + 1]);
                    if (result == -1) {
                        cout << "Error undefined escape sequence " << yytext[i + 1] << endl;
                        exit(0);
                    } else if (result == 0) {
                        if (keepWritingOutput) {
                            // We encountered a \0, so the string ends here.
                            keepWritingOutput = false;
//                            output.push_back(result);
                            i += 1;
                        }
                    } else {
                        if (keepWritingOutput) {
                            output.push_back(result);
                            i += 1;
                        }
                    }
                }
            }
        }
        cout << yylineno << " " << TOKEN_NAMES[token] << " " << output << endl;
    } else {
        // this prints out any other token that wasn't matched before
        cout << yylineno << " " << TOKEN_NAMES[token] << " " << yytext << endl;
    }
}

int main() {
    int token;
    while (token = yylex()) {
        showToken(token);
    }
    return 0;
}