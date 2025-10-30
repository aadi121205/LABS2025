// Implementation of CODE GENERATOR

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << argv[0] << " <input_file> [output_file]" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Error: Cannot open input file " << argv[1] << endl;
        return 1;
    }

    ofstream output;
    ostream* outPtr = &cout;
    if (argc == 3) {
        output.open(argv[2]);
        if (!output.is_open()) {
            cerr << "Error: Cannot open output file " << argv[2] << endl;
            return 1;
        }
        outPtr = &output;
    }

    int single_line_comments = 0;
    int multi_line_comments = 0;
    int total_comment_lines = 0;
    int total_comment_blocks = 0;

    string line;
    bool inBlockComment = false;

    while (getline(input, line)) {
        string result;
        size_t i = 0;
        bool lineHasComment = false;

        while (i < line.size()) {
            if (!inBlockComment && i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/') {
                // Single-line comment
                single_line_comments++;
                total_comment_blocks++;
                total_comment_lines++;
                lineHasComment = true;
                break; // Skip rest of the line
            } else if (!inBlockComment && i + 1 < line.size() && line[i] == '/' && line[i + 1] == '*') {
                // Start of multi-line comment
                inBlockComment = true;
                multi_line_comments++;
                total_comment_blocks++;
                lineHasComment = true;
                i += 2;
            } else if (inBlockComment && i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
                // End of multi-line comment
                inBlockComment = false;
                i += 2;
            } else {
                i++;
            }
        }

        if (inBlockComment || lineHasComment)
            total_comment_lines++;
            // Write the processed line
        *outPtr << result << endl;
    }
    *outPtr << "Single-line comments: " << single_line_comments << endl;
    *outPtr << "Multi-line comments: " << multi_line_comments << endl;
    *outPtr << "Total comment lines: " << total_comment_lines << endl;
    *outPtr << "Total comment blocks: " << total_comment_blocks << endl;

    input.close();
    if (output.is_open()) output.close();

    return 0;
}
