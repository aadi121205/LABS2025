#include <iostream>
#include <cctype>
using namespace std;

bool isToken(const string& str) {
    if (str.empty()) return false;
    if (!isalpha(str[0]) && str[0] != '_') return false;
    for (size_t i = 1; i < str.size(); ++i) {
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}

int main() {
    string input;
    cout << "Enter a string: ";
    cin >> input;
    if (isToken(input))
        cout << input << " is a valid token.\n";
    else
        cout << input << " is NOT a valid token.\n";
    return 0;
}