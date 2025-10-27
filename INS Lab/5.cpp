// To implement Hill- cipher encryption decryption

#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int MOD = 26;

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

vector<vector<int>> getCofactor(const vector<vector<int>>& mat, int p, int q, int n) {
    vector<vector<int>> temp(n - 1, vector<int>(n - 1));
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
    return temp;
}

int determinant(const vector<vector<int>>& mat, int n) {
    int D = 0;
    if (n == 1) {
        return mat[0][0];
    }

    int sign = 1;
    for (int f = 0; f < n; f++) {
        vector<vector<int>> temp = getCofactor(mat, 0, f, n);
        D += sign * mat[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return D;
}

vector<vector<int>> adjoint(const vector<vector<int>>& mat) {
    int n = mat.size();
    vector<vector<int>> adj(n, vector<int>(n));
    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }
    int sign = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<vector<int>> temp = getCofactor(mat, i, j, n);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign * determinant(temp, n - 1) + MOD) % MOD;
        }
    }
    return adj;
}

vector<vector<int>> inverse(const vector<vector<int>>& mat) {
    int n = mat.size();
    int det = determinant(mat, n);
    det = (det % MOD + MOD) % MOD; // Ensure positive determinant

    int detInverse = modInverse(det, MOD);
    if (detInverse == -1) {
        cerr << "Matrix is not invertible. Cannot decrypt." << endl;
        exit(1);
    }

    vector<vector<int>> adj = adjoint(mat);
    vector<vector<int>> inv(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = (adj[i][j] * detInverse) % MOD;
        }
    }
    return inv;
}

string encrypt(const string& text, const vector<vector<int>>& keyMatrix) {
    string result = "";
    int n = keyMatrix.size();
    string processedText = "";

    for (char c : text) {
        if (isalpha(c)) {
            processedText += toupper(c);
        }
    }

    // Pad with 'X' if length is not a multiple of n
    while (processedText.length() % n != 0) {
        processedText += 'X';
    }

    for (size_t i = 0; i < processedText.length(); i += n) {
        vector<int> block(n);
        for (int j = 0; j < n; j++) {
            block[j] = processedText[i + j] - 'A';
        }

        vector<int> cipherBlock(n, 0);
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                cipherBlock[j] = (cipherBlock[j] + keyMatrix[j][k] * block[k]) % MOD;
            }
        }

        for (int j = 0; j < n; j++) {
            result += (char)(cipherBlock[j] + 'A');
        }
    }
    return result;
}

string decrypt(const string& ciphertext, const vector<vector<int>>& keyMatrix) {
    vector<vector<int>> invKeyMatrix = inverse(keyMatrix);
    return encrypt(ciphertext, invKeyMatrix); // Decryption is encryption with inverse key
}

int main() {
    vector<vector<int>> key = {{9, 4}, {5, 7}};
    string text = "HIIAMINPAINH";
    string encrypted = encrypt(text, key);
    string decrypted = decrypt(encrypted, key);
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << decrypted << endl;
    return 0;
}