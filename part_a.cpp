#include "tokens.hpp"

extern int yylex();

void showToken(const int token) {

}

int main()
{
	int token;
	while(token = yylex()) {
	    showToken(token);
	}
	return 0;
}