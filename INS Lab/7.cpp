// RSA Encryption and Decryption in C++ by Aaditya Bhatia 23/CS/004

#include <iostream>
#include <vector>
using namespace std;
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(a, b % a);
}
long long modPow(long long base, long long exp, long long mod)
{
    long long result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}
int modInverse(int e, int phi)
{
    for (int d = 2; d < phi; d++)
    {
        if ((e * d) % phi == 1)
            return d;
    }
    return -1;
}
int main()
{
    int p = 61;
    int q = 53;
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 17;
    int d = modInverse(e, phi);
    if (d == -1)
    {
        cout << "No modular inverse found!" << endl;
        return 0;
    }
    cout << "Public Key: (" << e << ", " << n << ")" << endl;
    cout << "Private Key: (" << d << ", " << n << ")" << endl;
    string plaintext = "HELLO";
    vector<long long> ciphertext;
    for (char c : plaintext)
    {
        ciphertext.push_back(modPow(c, e, n));
    }
    for (long long c : ciphertext)
        cout << c << " ";
    cout << endl;
    string decryptedText = "";
    for (long long c : ciphertext)
    {
        decryptedText += (char)modPow(c, d, n);
    }
    cout << decryptedText << endl;
    return 0;
}
