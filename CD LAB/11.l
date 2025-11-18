%{
#include "expr.tab.h"
%}

%%
[0-9]+              { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*   { return ID; }
[\+\-\*\/]          { return yytext[0]; }
[\n]                { return '\n'; }
[ \t]               { /* ignore whitespace */ }
.                   { printf("Invalid character: %s\n", yytext); }
%%
int yywrap() {
    return 1;
}
