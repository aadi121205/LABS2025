// Implementation of OPERATOR PRECEDENCE PARSER
#include <bits/stdc++.h>
using namespace std;

vector<char> symbols = {'+', '-', '*', '/', 'i', '$'};

vector<vector<char>> precedence = {
    // +    -    *    /    i    $
    {'>', '>', '<', '<', '<', '>'}, // +
    {'>', '>', '<', '<', '<', '>'}, // -
    {'>', '>', '>', '>', '<', '>'}, // *
    {'>', '>', '>', '>', '<', '>'}, // /
    {'>', '>', '>', '>', ' ', '>'}, // i
    {'<', '<', '<', '<', '<', '='}  // $
};

int indexOf(char c) {
    for (int i = 0; i < symbols.size(); i++)
        if (symbols[i] == c) return i;
    return -1;
}

char getRelation(char a, char b) {
    int i = indexOf(a);
    int j = indexOf(b);
    if (i == -1 || j == -1) return ' ';
    return precedence[i][j];
}

int main() {
    string input;
    cout << "Enter the expression (end with $): ";
    cin >> input;

    if (input.back() != '$'){
        cout<<"Expression is not terminated by $, try again"<<endl;
        exit(0);
    }
    queue<char> see;
    for(int i=0;i<input.size();i++){
        see.push(input[i]);
    }
    string st="$";
    int i=0;

    cout<<endl<<"STACK"<<"  "<<"INPUT"<<"  "<<"ACTION"<<endl;
    cout<<"------------------------------------------"<<endl;

    while (true) {
        char a = st.back();
        char b = input[i];

        

        cout << st << "  " << input.substr(i) << "  ";

        char relation = getRelation(a, b);
        if (a == '$' && b == '$') {
            cout << "Accept" << endl;
            break;
        }

        if (relation == '<' || relation == '=') {
            cout << "Shift " << b << endl;
            st.push_back(b);
            i++;
        } 
        else if (relation == '>') {
            cout << "Reduce" << endl;
            while (!st.empty()) {
                char top = st.back();
                st.pop_back();
                if (!st.empty() && getRelation(st.back(), top) == '<')
                    break;
            }
        } 
        
        else {
            cout << "Error: invalid expression\n";
            break;
        }
    }
    return 0;
}