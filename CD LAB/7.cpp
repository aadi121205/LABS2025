#include <iostream>
#include <string>
using namespace std;

string input;
int index_pos = 0;

void E();
void E_prime();
void T();
void T_prime();
void F();

void match(char expected) {
    if (index_pos < input.length() && input[index_pos] == expected) {
        index_pos++;
    } else {
        throw runtime_error(
            "Expression Rejected: Expected '" + string(1, expected) +
            "' but found '" +
            (index_pos < input.length() ? string(1, input[index_pos]) : "end of string") + "'");
    }
}

// Grammar Rule: E -> T E'
void E() {
    cout << "E -> T E'" << endl;
    T();
    E_prime();
}

// Grammar Rule: E' -> + T E' | ε
void E_prime() {
    if (index_pos < input.length() && input[index_pos] == '+') {
        cout << "E' -> + T E'" << endl;
        match('+');
        T();
        E_prime();
    } else {
        cout << "E' -> ε" << endl;
    }
}

// Grammar Rule: T -> F T'
void T() {
    cout << "T -> F T'" << endl;
    F();
    T_prime();
}

// Grammar Rule: T' -> * F T' | ε
void T_prime() {
    if (index_pos < input.length() && input[index_pos] == '*') {
        cout << "T' -> * F T'" << endl;
        match('*');
        F();
        T_prime();
    } else {
        cout << "T' -> ε" << endl;
    }
}

// Grammar Rule: F -> ( E ) | i
void F() {
    if (index_pos < input.length() && input[index_pos] == '(') {
        cout << "F -> ( E )" << endl;
        match('(');
        E();
        match(')');
    } else if (index_pos < input.length() && input[index_pos] == 'i') {
        cout << "F -> i" << endl;
        match('i');
    } else {
        throw runtime_error("Expression Rejected: Syntax error at index " + to_string(index_pos));
    }
}

int main() {
    cout << "Enter an expression (e.g., i+i*i or (i+i)*i):" << endl;
    getline(cin, input);

    try {
        E();
        if (index_pos == input.length()) {
            cout << "\nExpression Accepted!" << endl;
        } else {
            throw runtime_error("Expression Rejected: Extra characters at the end.");
        }
    } catch (const runtime_error& e) {
        cout << "\n" << e.what() << endl;
    }

    return 0;
}