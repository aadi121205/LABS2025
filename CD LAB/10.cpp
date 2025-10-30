// Program using LEX to count the numbers of comment lines, identifiers, characters and words

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
    {
        if (argc != 2)
            {
                cerr << "Usage: " << argv[0] << " <input_file>" << endl;
                return 1;
            }
        ifstream input(argv[1]);
        if (!input.is_open())
            {
                cerr << "Error: Cannot open input file " << argv[1] << endl;
                return 1;
            }
        ofstream output("output.txt");
        if (!output.is_open())
            {
                cerr << "Error: Cannot open output file output.txt" << endl;
                return 1;
            }
        int identifier_count = 0;
        int character_count = 0;
        int word_count = 0;
        string word;
    }