#include <bits/stdc++.h>
using namespace std;

// Function to format the key to match text length
string generateKey(const string &text, const string &key) {
    string newKey;
    int keyLen = key.size();
    for (int i = 0, j = 0; i < (int)text.size(); i++) {
        if (isalpha(text[i])) {
            newKey += toupper(key[j % keyLen]);
            j++;
        } else {
            newKey += text[i]; // keep spaces/punctuation
        }
    }
    return newKey;
}

// Encrypt using Vigenère cipher
string encrypt(const string &text, const string &key) {
    string result;
    for (int i = 0; i < (int)text.size(); i++) {
        if (isupper(text[i])) {
            result += char((text[i] - 'A' + (key[i] - 'A')) % 26 + 'A');
        } else if (islower(text[i])) {
            result += char((text[i] - 'a' + (tolower(key[i]) - 'a')) % 26 + 'a');
        } else {
            result += text[i];
        }
    }
    return result;
}

// Decrypt using Vigenère cipher
string decrypt(const string &cipher, const string &key) {
    string result;
    for (int i = 0; i < (int)cipher.size(); i++) {
        if (isupper(cipher[i])) {
            result += char((cipher[i] - 'A' - (key[i] - 'A') + 26) % 26 + 'A');
        } else if (islower(cipher[i])) {
            result += char((cipher[i] - 'a' - (tolower(key[i]) - 'a') + 26) % 26 + 'a');
        } else {
            result += cipher[i];
        }
    }
    return result;
}

int main() {
    string text, key;
    int choice;

    cout << "Enter text: ";
    getline(cin, text);
    cout << "Enter key (letters only): ";
    cin >> key;

    // Make key uppercase for internal use
    for (auto &c : key) c = toupper(c);

    cout << "1. Encrypt\n2. Decrypt\nChoice: ";
    cin >> choice;
    cin.ignore();

    string formattedKey = generateKey(text, key);

    if (choice == 1) {
        cout << "Encrypted: " << encrypt(text, formattedKey) << "\n";
    } else if (choice == 2) {
        cout << "Decrypted: " << decrypt(text, formattedKey) << "\n";
    } else {
        cout << "Invalid choice.\n";
    }

    return 0;
}
