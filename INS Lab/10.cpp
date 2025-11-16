// Implement a digital signature algorithm.
// By Aaditya Bhatia 23/CS/004

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
using namespace std;

// ---------------- SHA-1 IMPLEMENTATION ----------------

typedef uint32_t uint32;
typedef uint64_t uint64;

uint32 leftRotate(uint32 value, int count) {
    return (value << count) | (value >> (32 - count));
}

string sha1(const string &input) {
    const uint32 H0 = 0x67452301;
    const uint32 H1 = 0xEFCDAB89;
    const uint32 H2 = 0x98BADCFE;
    const uint32 H3 = 0x10325476;
    const uint32 H4 = 0xC3D2E1F0;

    vector<uint8_t> message(input.begin(), input.end());
    uint64 originalBitLen = message.size() * 8;

    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448)
        message.push_back(0x00);

    for (int i = 7; i >= 0; --i)
        message.push_back((originalBitLen >> (i * 8)) & 0xFF);

    uint32 h0 = H0, h1 = H1, h2 = H2, h3 = H3, h4 = H4;

    for (size_t chunkOffset = 0; chunkOffset < message.size(); chunkOffset += 64) {
        uint32 w[80];

        for (int i = 0; i < 16; ++i) {
            w[i] = (message[chunkOffset + 4*i] << 24) |
                   (message[chunkOffset + 4*i + 1] << 16) |
                   (message[chunkOffset + 4*i + 2] << 8) |
                   (message[chunkOffset + 4*i + 3]);
        }

        for (int i = 16; i < 80; ++i)
            w[i] = leftRotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

        uint32 a = h0, b = h1, c = h2, d = h3, e = h4;

        for (int i = 0; i < 80; ++i) {
            uint32 f, k;

            if (i < 20)      { f = (b & c) | (~b & d); k = 0x5A827999; }
            else if (i < 40) { f = b ^ c ^ d;          k = 0x6ED9EBA1; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
            else             { f = b ^ c ^ d;          k = 0xCA62C1D6; }

            uint32 temp = leftRotate(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    stringstream ss;
    ss << hex << uppercase << setfill('0')
       << setw(8) << h0 << setw(8) << h1 << setw(8) << h2
       << setw(8) << h3 << setw(8) << h4;

    return ss.str();
}

// ---------------- RSA HELPERS ----------------

long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int modInverse(int e, int phi) {
    for (int d = 2; d < phi; d++)
        if ((1LL * e * d) % phi == 1)
            return d;
    return -1;
}

long long hashToInt(const string &hash, long long mod) {
    long long value = 0;
    for (char c : hash) {
        int v;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'A' && c <= 'F') v = 10 + (c - 'A');
        else v = 0;

        value = (value * 16 + v) % mod;
    }
    return value;
}

// ---------------- MAIN ----------------

int main() {
    int p = 61, q = 53;
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 17;
    int d = modInverse(e, phi);

    cout << "Public Key: (" << e << ", " << n << ")\n";
    cout << "Private Key: (" << d << ", " << n << ")\n";

    string message;
    cout << "Enter message to sign: ";
    getline(cin, message);

    string hash = sha1(message);
    cout << "SHA-1 Hash: " << hash << endl;

    long long hashInt = hashToInt(hash, n);
    long long signature = modPow(hashInt, d, n);

    cout << "Digital Signature: " << signature << endl;

    long long verify = modPow(signature, e, n);
    cout << "Verification Value: " << verify << endl;

    if (verify == hashInt)
        cout << "Signature Verified!" << endl;
    else
        cout << "Verification Failed!" << endl;

    return 0;
}
