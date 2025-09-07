// made by Aadi
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
                if (!inBlockComment) {
                    result.push_back(line[i]);
                } else {
                    lineHasComment = true;
                }
                i++;
            }
        }

        if (inBlockComment || lineHasComment)
            total_comment_lines++;

        // Write the processed line
        if (!result.empty() && !inBlockComment) {
            *outPtr << result << "\n";
        } else if (!inBlockComment && result.empty()) {
            *outPtr << "\n";
        }
    }

    input.close();
    if (output.is_open()) output.close();

    // Print statistics
    cerr << "\n=== Comment Analysis Report ===\n";
    cerr << "Single-line comments (//) removed: " << single_line_comments << "\n";
    cerr << "Multi-line comments (/* */) removed: " << multi_line_comments << "\n";
    cerr << "Total comment blocks removed: " << total_comment_blocks << "\n";
    cerr << "Total comment lines removed: " << total_comment_lines << "\n";

    if (argc == 3) {
        cout << "Program without comments has been written to " << argv[2] << endl;
    }

    return 0;
}
