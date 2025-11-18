%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER ID

%%
input: /* empty */
     | input line
     ;

line: expr '\n'   { printf("Valid expression\n"); }
    | '\n'
    ;

expr: term
    | expr '+' term
    | expr '-' term
    ;

term: factor
    | term '*' factor
    | term '/' factor
    ;

factor: NUMBER
      | ID
      | '(' expr ')'
      ;
%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}
