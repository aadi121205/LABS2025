// To implement Polyalphabetic cipher encryption decryption. Encryption/Decryption: Based on substitution, using multiple substitution Alphabets
// By Aaditya Bhatia 23/CS/004

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

string toUpper(string s) {
 for (char &c : s) {
 c = toupper(c);
 }
 return s;
}

string encrypt(string plain_text, string key) {
    string cipher_text = "";
    key = toUpper(key);
 int key_len = key.length();
 int key_idx = 0;

    for (char c : plain_text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            cipher_text += char(((c - base + (key[key_idx % key_len] - 'A')) % 26) + base);
            key_idx++;
        } else {
            cipher_text += c;
        }
    }
    return cipher_text;
}

string decrypt(string cipher_text, string key) {
    string plain_text = "";
    key = toUpper(key);
 int key_len = key.length();
 int key_idx = 0;

    for (char c : cipher_text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            plain_text += char(((c - base - (key[key_idx % key_len] - 'A') + 26) % 26) + base);
            key_idx++;
        } else {
            plain_text += c;
        }
    }
    return plain_text;
}

int main() {
    string plain_text, key;
    cout << "Enter Text :";
    cin >> plain_text;
    cout << "Enter Key :";
    cin >> key;
    string cipher_text;
    cipher_text = encrypt(plain_text, key);
    cout << "Cipher Text :" << cipher_text << endl;
    cout << "Decryption :" << decrypt(cipher_text, key) << endl;
    return 0;
}