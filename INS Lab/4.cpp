// To implement Polyalphabetic cipher encryption decryption. Encryption/Decryption: Based on substitution, using multiple substitution Alphabets

#include <iostream>
#include <string>
using namespace std;

string encrypt(string text, string key) {
    string result = "";
    int keyLength = key.length();
    for (int i = 0, j = 0; i < text.length(); i++) {
        char c = text[i];
        if (isalpha(c)) {
            char offset = isupper(c) ? 'A' : 'a';
            result += (c + toupper(key[j % keyLength]) - 2 * offset) % 26 + offset;
            j++;
        } else {
            result += c;
        }
    }
    return result;
}

string decrypt(string text, string key) {
    string result = "";
    int keyLength = key.length();
    for (int i = 0, j = 0; i < text.length(); i++) {
        char c = text[i];
        if (isalpha(c)) {
            char offset = isupper(c) ? 'A' : 'a';
            result += (c - toupper(key[j % keyLength]) + 26) % 26 + offset;
            j++;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    string text, key;
    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the key: ";
    cin >> key;

    string encryptedText = encrypt(text, key);
    cout << "Encrypted Text: " << encryptedText << endl;

    string decryptedText = decrypt(encryptedText, key);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}
