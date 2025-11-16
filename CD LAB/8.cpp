// Implementation of CODE OPTIMIZATION TECHNIQUES
// code by Aaditya

#include <bits/stdc++.h>

using namespace std;

class ThreeAddressCode {
public:
    string result;
    string operand1;
    string op;
    string operand2;
    bool isAssignment;

    ThreeAddressCode(string result, string operand1, string op, string operand2) {
        this->result = result;
        this->operand1 = operand1;
        this->op = op;
        this->operand2 = operand2;
        this->isAssignment = (op == "=" && operand2 == "");
    }

    string toString() const {
        if (isAssignment) {
            return result + " = " + operand1;
        } else if (operand2.empty()) {
            return result + " = " + op + operand1;
        } else {
            return result + " = " + operand1 + " " + op + " " + operand2;
        }
    }
};

bool isNumeric(const string &str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c) && c != '-') return false;
    }
    return true;
}

ThreeAddressCode parseStatement(const string &line) {
    size_t eqPos = line.find('=');
    string result = line.substr(0, eqPos);
    string rhs = line.substr(eqPos + 1);

    auto trim = [](string s) {
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
        return s;
    };

    result = trim(result);
    rhs = trim(rhs);

    vector<string> ops = {"+", "-", "*", "/"};
    for (string op : ops) {
        size_t pos = rhs.find(op);
        if (pos != string::npos) {
            string operand1 = trim(rhs.substr(0, pos));
            string operand2 = trim(rhs.substr(pos + 1));
            return ThreeAddressCode(result, operand1, op, operand2);
        }
    }

    return ThreeAddressCode(result, rhs, "=", "");
}

void printCode(const vector<ThreeAddressCode> &code) {
    for (auto &tac : code) {
        cout << "  " << tac.toString() << endl;
    }
}

vector<ThreeAddressCode> constantFolding(const vector<ThreeAddressCode> &code) {
    vector<ThreeAddressCode> result;
    for (auto &tac : code) {
        if (isNumeric(tac.operand1) && isNumeric(tac.operand2)) {
            int val1 = stoi(tac.operand1);
            int val2 = stoi(tac.operand2);
            int resultVal = 0;
            if (tac.op == "+") resultVal = val1 + val2;
            else if (tac.op == "-") resultVal = val1 - val2;
            else if (tac.op == "*") resultVal = val1 * val2;
            else if (tac.op == "/") resultVal = val1 / val2;
            result.emplace_back(tac.result, to_string(resultVal), "=", "");
        } else {
            result.push_back(tac);
        }
    }
    return result;
}

vector<ThreeAddressCode> constantPropagation(const vector<ThreeAddressCode> &code) {
    map<string, string> constants;
    vector<ThreeAddressCode> result;
    for (auto &tac : code) {
        string op1 = constants.count(tac.operand1) ? constants[tac.operand1] : tac.operand1;
        string op2 = constants.count(tac.operand2) ? constants[tac.operand2] : tac.operand2;
        ThreeAddressCode newTac(tac.result, op1, tac.op, op2);
        result.push_back(newTac);
        if (tac.isAssignment && isNumeric(op1)) {
            constants[tac.result] = op1;
        }
    }
    return result;
}

vector<ThreeAddressCode> deadCodeElimination(const vector<ThreeAddressCode> &code) {
    set<string> usedVars;
    for (auto &tac : code) {
        if (!tac.operand1.empty()) usedVars.insert(tac.operand1);
        if (!tac.operand2.empty()) usedVars.insert(tac.operand2);
    }
    vector<ThreeAddressCode> result;
    for (auto &tac : code) {
        if (usedVars.count(tac.result) || tac.result == "result" || tac.result[0] != 't') {
            result.push_back(tac);
        }
    }
    return result;
}

vector<ThreeAddressCode> algebraicSimplification(const vector<ThreeAddressCode> &code) {
    vector<ThreeAddressCode> result;
    for (auto &tac : code) {
        bool simplified = false;

        if (tac.op == "+") {
            if (tac.operand1 == "0") {
                result.emplace_back(tac.result, tac.operand2, "=", "");
                simplified = true;
            } else if (tac.operand2 == "0") {
                result.emplace_back(tac.result, tac.operand1, "=", "");
                simplified = true;
            }
        }

        if (tac.op == "-" && tac.operand2 == "0") {
            result.emplace_back(tac.result, tac.operand1, "=", "");
            simplified = true;
        }

        if (tac.op == "*") {
            if (tac.operand1 == "1") {
                result.emplace_back(tac.result, tac.operand2, "=", "");
                simplified = true;
            } else if (tac.operand2 == "1") {
                result.emplace_back(tac.result, tac.operand1, "=", "");
                simplified = true;
            } else if (tac.operand1 == "0" || tac.operand2 == "0") {
                result.emplace_back(tac.result, "0", "=", "");
                simplified = true;
            }
        }

        if (tac.op == "/" && tac.operand2 == "1") {
            result.emplace_back(tac.result, tac.operand1, "=", "");
            simplified = true;
        }

        if (!simplified) result.push_back(tac);
    }
    return result;
}

int main() {
    cout << "=== CODE OPTIMIZATION TECHNIQUES ===\n";
    cout << "Enter number of three-address code statements: ";
    int n;
    cin >> n;
    cin.ignore();

    vector<ThreeAddressCode> code;
    cout << "Enter statements (format: t1 = a + b or t1 = 5):\n";
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        code.push_back(parseStatement(line));
    }

    cout << "\n--- ORIGINAL CODE ---\n";
    printCode(code);

    cout << "\n--- APPLYING OPTIMIZATIONS ---\n";

    cout << "\n1. Constant Folding:\n";
    auto optimized = constantFolding(code);
    printCode(optimized);

    cout << "\n2. Constant Propagation:\n";
    optimized = constantPropagation(optimized);
    printCode(optimized);

    cout << "\n3. Dead Code Elimination:\n";
    optimized = deadCodeElimination(optimized);
    printCode(optimized);

    cout << "\n4. Algebraic Simplification:\n";
    optimized = algebraicSimplification(optimized);
    printCode(optimized);

    cout << "\n--- FINAL OPTIMIZED CODE ---\n";
    printCode(optimized);
}
