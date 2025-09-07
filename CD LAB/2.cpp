// made by Aadi
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    // Check if filename is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];

    // Open the file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file '" << filename << "'." << endl;
        return 1;
    }

    int spaceCount = 0, lineCount = 0;
    string line;

    // Read file line by line
    while (getline(file, line)) {
        ++lineCount;
        for (char ch : line) {
            if (ch == ' ') {
                ++spaceCount;
            }
        }
    }

    file.close();

    // Output the results
    cout << "Spaces: " << spaceCount << endl;
    cout << "Lines: " << lineCount << endl;

    return 0;
}
