%{
#include <stdio.h>
int commentCount = 0;
FILE *out;
%}
%x COMMENT
%%
"//".*                   { commentCount++; }   /* Single-line comment */

"/*"                     { commentCount++; BEGIN(COMMENT); }  /* Start multi-line comment */

<COMMENT>"*/"            { BEGIN(INITIAL); }   /* End multi-line comment */

<COMMENT>.|\n             ;                    /* Ignore everything inside multi-line comment */

.|\n                     { fputc(yytext[0], out); }   /* Copy everything else */

%%

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("Could not open input file %s\n", argv[1]);
        return 1;
    }

    out = fopen(argv[2], "w");
    if (!out) {
        printf("Could not open output file %s\n", argv[2]);
        fclose(in);
        return 1;
    }

    yyin = in;
    yylex();

    printf("Total number of comment lines: %d\n", commentCount);

    fclose(in);
    fclose(out);
    return 0;
}
int yywrap() {
    return 1;
}
