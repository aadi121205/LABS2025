#include <iostream>
#include <string>
using namespace std;

string encrypt(string text, int s) {
    string result = "";

    for (int i = 0; i < text.length(); i++) {
        if (isupper(text[i])) {
            result += char(int(text[i] - 'A' + s) % 26 + 'A');
        }
        else if (islower(text[i])) {
            result += char(int(text[i] - 'a' + s) % 26 + 'a');
        }
        else {
            result += text[i]; // Keep non-alphabetic characters unchanged
        }
    }
    return result;
}

string decrypt(string text, int s) {
    return encrypt(text, 26 - (s % 26));     // Decrypt by shifting backwards
}

int main() {
    string text;
    int s;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter shift value: ";
    cin >> s;

    string encrypted = encrypt(text, s);
    string decrypted = decrypt(encrypted, s);

    cout << "\nOriginal : " << text;
    cout << "\nShift    : " << s;
    cout << "\nEncrypted: " << encrypted;
    cout << "\nDecrypted: " << decrypted << endl;

    return 0;
}
