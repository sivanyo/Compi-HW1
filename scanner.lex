%{
/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap
digit   ([1-9])
letter  ([a-zA-Z])
whitespace  ([\r\n\t ])
hexchar ([0-9a-fA-F])
escape  (\\n|\\r|\\t|\\0|\\x{hexchar}{2}|\\.)
%x          STRINGS_TERM

%%

void                                                                                                                                                return VOID;
int                                                                                                                                                 return INT;
byte                                                                                                                                                return BYTE;
b                                                                                                                                                   return B;
bool                                                                                                                                                return BOOL;
and                                                                                                                                                 return AND;
or                                                                                                                                                  return OR;
not                                                                                                                                                 return NOT;
true                                                                                                                                                return TRUE;
false                                                                                                                                               return FALSE;
return                                                                                                                                              return RETURN;
if                                                                                                                                                  return IF;
else                                                                                                                                                return ELSE;
while                                                                                                                                               return WHILE;
break                                                                                                                                               return BREAK;
continue                                                                                                                                            return CONTINUE;
switch                                                                                                                                              return SWITCH;
case                                                                                                                                                return CASE;
default                                                                                                                                             return DEFAULT;
(\:)                                                                                                                                                return COLON;
(\;)                                                                                                                                                return SC;
(\,)                                                                                                                                                return COMMA;
(\()                                                                                                                                                return LPAREN;
(\))                                                                                                                                                return RPAREN;
(\{)                                                                                                                                                return LBRACE;
(\})                                                                                                                                                return RBRACE;
(=)                                                                                                                                                 return ASSIGN;
((<|>|=|!)=)|>|<                                                                                                                                    return RELOP;
(\+|\-|\*|\/)                                                                                                                                       return BINOP;
\/\/([\x00-\x09\x0b-\x0c\x0e-\x7f])*                                                                                                                return COMMENT;
{letter}({letter}|{digit}|0)*                                                                                                                       return ID;
({digit}({digit}|0)*)|0                                                                                                                             return NUM;
{whitespace}                                                                                                                                        ;
(\")                                                                                                                                                BEGIN(STRINGS_TERM);
<STRINGS_TERM>([\x00-\x09\x0b-\x0c\x0e-\x21\x23-\x5b\x5d-\x7f]|{escape})*(\")                                                                       {BEGIN(INITIAL); return STRING;}
<STRINGS_TERM>([^(\")]+)                                                                                                                            return UNCLOSEDSTRING;
<STRINGS_TERM><<EOF>>                                                                                                                               return UNCLOSEDSTRING;
.                                                                                                                                                   return ERRORCHAR;

%%