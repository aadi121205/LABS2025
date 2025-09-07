#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    ifstream file(filename);
    if (!file) {
        cout << "Cannot open file.\n";
        return 1;
    }
    int spaceCount = 0, lineCount = 0;
    string line;
    while (getline(file, line)) {
        ++lineCount;
        for (char ch : line) {
            if (ch == ' ') ++spaceCount;
        }
    }
    file.close();
    cout << "Spaces: " << spaceCount << endl;
    cout << "Lines: " << lineCount << endl;
    return 0;
}