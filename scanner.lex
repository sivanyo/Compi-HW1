%{
/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap
digit   ([1-9])
letter  ([a-zA-Z])
whitespace  ([\r\n\t])

// comment for <STRINGS><<EOF>> If we reach a string like \"EOF, this is a bad string
// comment for normal STRINGS parsing Try to match a complete string text all the way to the ending \", match \\,\",\n,\r,\t,\0,\x separately
// comment for bad string ending with \" Try to match 1 or more instances of \" which indicate a string of the format "\" which is illegal
//<STRINGS>([^(\")*((\")?)

%% // start of rules section

void                                                                                                                                        return VOID;
int                                                                                                                                         return INT;
byte                                                                                                                                        return BYTE;
b                                                                                                                                           return B;
bool                                                                                                                                        return BOOL;
and                                                                                                                                         return AND;
or                                                                                                                                          return OR;
not                                                                                                                                         return NOT;
true                                                                                                                                        return TRUE;
false                                                                                                                                       return FALSE;
return                                                                                                                                      return RETURN;
if                                                                                                                                          return IF;
else                                                                                                                                        return ELSE;
while                                                                                                                                       return WHILE;
break                                                                                                                                       return BREAK;
continue                                                                                                                                    return CONTINUE;
switch                                                                                                                                      return SWITCH;
case                                                                                                                                        return CASE;
default                                                                                                                                     return DEFAULT;
(\:)                                                                                                                                        return COLON;
(\;)                                                                                                                                        return SC;
(\,)                                                                                                                                        return COMMA;
(\()                                                                                                                                        return LPAREN;
(\))                                                                                                                                        return RPAREN;
(\{)                                                                                                                                        return LBRACE;
(\})                                                                                                                                        return RBRACE;
(=)                                                                                                                                         return ASSIGN;
((<|>|=|!)=)|>|<                                                                                                                            return RELOP;
(\+|\-|\*|\/)                                                                                                                               return BINOP;
\/\/([\x00-\x09\x0b-\x0c\0xe-\x7f])*                                                                                                        return COMMENT;
{letter}({letter}|{digit}|0)*                                                                                                               return ID;
{digit}({digit}|0)*                                                                                                                         return NUM;
(\")                                                                                                                                        BEGIN(STRINGS);
<STRINGS><<EOF>>                                                                                                                            return ERRORSTRING;
<STRINGS>([\x00-\x09\x0b-\x0c\x0e-\x21\x23-\x5b\x5d-\x7f]|(\\)(\\)|(\\)(\")|(\\)(n)|(\\)(r)|(\\)(t)|(\\)(0)|((\\)x[0-9|A-F][0-9|A-F]))*(\") {BEGIN(INITIAL); return STRING;}
<STRINGS>([^(\")]+)                                                                                                                         return ERRORSTRING;
.                                                                                                                                           return ERRORCHAR;

%%