// To implement DES sub key generation By Aaditya Bhatia 23/CS/004
#include <iostream>
#include <vector>
using namespace std;

int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int SHIFTS[16] = {
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
};

string permute(string input, int *table, int n) {
    string output = "";
    for (int i = 0; i < n; i++)
        output += input[table[i] - 1];
    return output;
}

string leftRotate(const string &key, int shifts) {
    return key.substr(shifts) + key.substr(0, shifts);
}

int main() {
    string key64;
    cout << "Enter 64-bit key (as binary string): ";
    cin >> key64;
    if (key64.size() != 64)
    {
        cerr << "Key must be 64 bits" << endl;
        return 1;
    }
    string key56 = permute(key64, PC1, 56);
    string C = key56.substr(0, 28);
    string D = key56.substr(28, 28);
    cout << "Round Subkeys:" << endl;
    for (int round = 0; round < 16; round++)
    {
        C = leftRotate(C, SHIFTS[round]);
        D = leftRotate(D, SHIFTS[round]);
        string CD = C + D;
        string subkey = permute(CD, PC2, 48);
        cout << "Round " << round + 1 << ": " << subkey << endl;
    }
    return 0;
}
