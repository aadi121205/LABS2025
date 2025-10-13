// To implement Play fair cipher encryption-decryption.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<vector<char>> char_matrix;

// Function to generate the 5x5 key matrix
void generateKeyMatrix(const string& key, char_matrix& matrix) {
    string key_processed;
    vector<bool> alphabet(26, false);
    matrix.assign(5, vector<char>(5));

    // Process the key: unique characters, uppercase, I/J combined
    for (char c : key) {
        if (isalpha(c)) {
            char upper_c = toupper(c);
            if (upper_c == 'J') upper_c = 'I';
            if (!alphabet[upper_c - 'A']) {
                key_processed += upper_c;
                alphabet[upper_c - 'A'] = true;
            }
        }
    }

    // Fill the rest of the matrix with the remaining alphabet
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (c == 'J') continue; // Skip J
        if (!alphabet[c - 'A']) {
            key_processed += c;
        }
    }

    // Populate the matrix
    int k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            matrix[i][j] = key_processed[k++];
        }
    }
}

// Function to find the position of a character in the matrix
void findPosition(const char_matrix& matrix, char ch, int& row, int& col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == ch) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Function to prepare the text for encryption
string prepareText(string text) {
    string prepared_text;
    // Convert to uppercase and remove non-alpha chars, J -> I
    for (char c : text) {
        if (isalpha(c)) {
            char upper_c = toupper(c);
            if (upper_c == 'J') upper_c = 'I';
            prepared_text += upper_c;
        }
    }

    // Insert 'X' between identical letters in a digraph
    for (size_t i = 0; i < prepared_text.length(); i += 2) {
        if (i + 1 < prepared_text.length()) {
            if (prepared_text[i] == prepared_text[i + 1]) {
                prepared_text.insert(i + 1, "X");
            }
        }
    }

    // If the text has an odd number of letters, append 'X'
    if (prepared_text.length() % 2 != 0) {
        prepared_text += 'X';
    }

    return prepared_text;
}

// Function to encrypt the text using the Playfair cipher
string encrypt(const string& text, const char_matrix& matrix) {
    string prepared_text = prepareText(text);
    string ciphertext;

    for (size_t i = 0; i < prepared_text.length(); i += 2) {
        char c1 = prepared_text[i];
        char c2 = prepared_text[i + 1];
        int r1, c1_pos, r2, c2_pos;

        findPosition(matrix, c1, r1, c1_pos);
        findPosition(matrix, c2, r2, c2_pos);

        if (r1 == r2) { // Same row
            ciphertext += matrix[r1][(c1_pos + 1) % 5];
            ciphertext += matrix[r2][(c2_pos + 1) % 5];
        } else if (c1_pos == c2_pos) { // Same column
            ciphertext += matrix[(r1 + 1) % 5][c1_pos];
            ciphertext += matrix[(r2 + 1) % 5][c2_pos];
        } else { // Rectangle
            ciphertext += matrix[r1][c2_pos];
            ciphertext += matrix[r2][c1_pos];
        }
    }
    return ciphertext;
}

// Function to decrypt the text using the Playfair cipher
string decrypt(const string& ciphertext, const char_matrix& matrix) {
    string plaintext;

    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        char c1 = ciphertext[i];
        char c2 = ciphertext[i + 1];
        int r1, c1_pos, r2, c2_pos;

        findPosition(matrix, c1, r1, c1_pos);
        findPosition(matrix, c2, r2, c2_pos);

        if (r1 == r2) { // Same row
            plaintext += matrix[r1][(c1_pos - 1 + 5) % 5];
            plaintext += matrix[r2][(c2_pos - 1 + 5) % 5];
        } else if (c1_pos == c2_pos) { // Same column
            plaintext += matrix[(r1 - 1 + 5) % 5][c1_pos];
            plaintext += matrix[(r2 - 1 + 5) % 5][c2_pos];
        } else { // Rectangle
            plaintext += matrix[r1][c2_pos];
            plaintext += matrix[r2][c1_pos];
        }
    }
    return plaintext;
}

int main() {
    string text, key;
    char_matrix matrix;

    cout << "Code By Aadi";

    cout << "Enter the key: ";
    getline(cin, key);

    cout << "Enter the text to encrypt: ";
    getline(cin, text);

    generateKeyMatrix(key, matrix);
    string encrypted_text = encrypt(text, matrix);
    string decrypted_text = decrypt(encrypted_text, matrix);

    cout << "\n--- Results ---\n";
    cout << "Original Text:    " << text << endl;
    cout << "Prepared Text:    " << prepareText(text) << endl;
    cout << "Encrypted Text:   " << encrypted_text << endl;
    cout << "Decrypted Text:   " << decrypted_text << endl;

    return 0;
}