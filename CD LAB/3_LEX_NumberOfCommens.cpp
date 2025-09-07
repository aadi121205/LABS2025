%{
/*
 * LEX Program to count the number of comment lines in a C program
 * Also eliminates them and copies the resulting program into separate file
 * 
 * To compile and run:
 * 1. Save this file as remove_comments.l
 * 2. Run: flex remove_comments.l
 * 3. Compile: gcc lex.yy.c -o remove_comments
 * 4. Usage: ./remove_comments < input.c > output.c
 */

#include <stdio.h>
#include <stdlib.h>

int single_line_comments = 0;
int multi_line_comments = 0;
int total_comment_lines = 0;
%}

%%

"//".*                      { 
                               single_line_comments++; 
                               total_comment_lines++;
                               /* Skip single-line comment */ 
                            }

"/*"([^*]|\*+[^*/])*\*+"/"  { 
                               multi_line_comments++;
                               total_comment_lines++;
                               /* Skip multi-line comment */ 
                            }

.                           { ECHO; }  /* Copy all other characters */
\n                          { ECHO; }  /* Copy newlines */

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    
    if (argc == 3) {
        /* Input and output files specified */
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        
        output_file = fopen(argv[2], "w");
        if (!output_file) {
            fprintf(stderr, "Error: Cannot create output file %s\n", argv[2]);
            fclose(input_file);
            return 1;
        }
        
        /* Redirect input and output */
        yyin = input_file;
        yyout = output_file;
    }
    else if (argc == 2) {
        /* Only input file specified, output to stdout */
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyin = input_file;
    }
    else {
        /* Use stdin and stdout */
        printf("Enter C program (Ctrl+D or Ctrl+Z to end):\n");
    }
    
    /* Process the input */
    yylex();
    
    /* Print statistics */
    fprintf(stderr, "\n=== Comment Analysis Report ===\n");
    fprintf(stderr, "Single-line comments (//) removed: %d\n", single_line_comments);
    fprintf(stderr, "Multi-line comments (/* */) removed: %d\n", multi_line_comments);
    fprintf(stderr, "Total comment blocks removed: %d\n", total_comment_lines);
    
    /* Close files if opened */
    if (input_file) fclose(input_file);
    if (output_file) fclose(output_file);
    
    if (argc == 3) {
        printf("Program without comments has been written to %s\n", argv[2]);
    }
    
    return 0;
}
