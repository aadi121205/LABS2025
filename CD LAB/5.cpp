// Implementation of SHIFT REDUCE PARSING ALGORITHM

#include <bits/stdc++.h>
using namespace std;

// Grammar: E -> E+E | E*E | (E) | i
vector<string> productions = {"E+E", "E*E", "(E)", "i"};

// Try to reduce the stack if any production matches
bool reduce(string &stk) {
    for (auto &prod : productions) {
        size_t pos = stk.find(prod);
        if (pos != string::npos) {
            stk.replace(pos, prod.size(), "E");
            cout << "REDUCE TO E\t" << stk << "\n";
            return true;
        }
    }
    return false;
}

int main() {
    string input;
    cout << "Enter the input string (e.g. i+i*i): ";
    cin >> input;
    input += "$";  // End marker

    string stk = "$"; // Start marker
    cout<<endl;
    cout << "STACK"<<"  "<<"INPUT"<<"  "<<"ACTION"<<endl;
    cout << "--------------------------------------"<<endl;

    // Continue until input is exhausted
    while (input.size() > 0) {
        // SHIFT
        stk += input[0];
        input.erase(input.begin());
        cout << stk << "  " << input << "  "<< "SHIFT"<<endl;

        // Try reducing as much as possible
        bool reduced = true;
        while (reduced)
            reduced = reduce(stk);
    }

    // Final acceptance check
    if (stk == "$E$"){
        cout<<endl;
        cout << "Input string accepted."<<endl;
    }
    else{
        cout<<endl;
        cout << "Input string rejected."<<endl;
    }

    return 0;
}