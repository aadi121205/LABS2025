%{
#include <stdio.h>
int comment_lines = 0;
int identifiers = 0;
int characters = 0;
int words = 0;
int in_multiline_comment = 0;
%}

%option noyywrap
%x COMMENT

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

%%

"//".*                  { comment_lines++; }
"/*"                    { in_multiline_comment = 1; comment_lines++; BEGIN(COMMENT); }

<COMMENT>"*/"           { in_multiline_comment = 0; BEGIN(INITIAL); }
<COMMENT>\n             { comment_lines++; }
<COMMENT>.              { }

{IDENTIFIER}            { identifiers++; words++; characters += yyleng; }

\n                      { }
.                       { characters++; }

%%

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("Error: Could not open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    
    yylex();
    
    printf("\n=== STATISTICS ===\n");
    printf("Comment Lines: %d\n", comment_lines);
    printf("Identifiers: %d\n", identifiers);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);
    
    if (argc > 1) {
        fclose(yyin);
    }
    
    return 0;
}
