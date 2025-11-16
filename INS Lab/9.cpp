// Write a program to generate SHA-1 hash.
// By Aaditya Bhatia 23/CS/004

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>

using namespace std;

typedef uint32_t uint32;
typedef uint64_t uint64;

const uint32 H0 = 0x67452301;
const uint32 H1 = 0xEFCDAB89;
const uint32 H2 = 0x98BADCFE;
const uint32 H3 = 0x10325476;
const uint32 H4 = 0xC3D2E1F0;

uint32 leftRotate(uint32 value, int count) {
    return (value << count) | (value >> (32 - count));
}

string sha1(const string &input) {
    vector<uint8_t> message(input.begin(), input.end());
    uint64 originalBitLen = static_cast<uint64>(message.size()) * 8;

    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448) {
        message.push_back(0x00);
    }

    for (int i = 7; i >= 0; --i) {
        message.push_back((originalBitLen >> (i * 8)) & 0xFF);
    }

    uint32 h0 = H0;
    uint32 h1 = H1;
    uint32 h2 = H2;
    uint32 h3 = H3;
    uint32 h4 = H4;

    for (size_t chunkOffset = 0; chunkOffset < message.size(); chunkOffset += 64) {
        uint32 w[80];

        for (int i = 0; i < 16; ++i) {
            size_t j = chunkOffset + i * 4;
            w[i] = (static_cast<uint32>(message[j]) << 24) |
                   (static_cast<uint32>(message[j + 1]) << 16) |
                   (static_cast<uint32>(message[j + 2]) << 8) |
                   (static_cast<uint32>(message[j + 3]));
        }

        for (int i = 16; i < 80; ++i) {
            w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }

        uint32 a = h0;
        uint32 b = h1;
        uint32 c = h2;
        uint32 d = h3;
        uint32 e = h4;

        for (int i = 0; i < 80; ++i) {
            uint32 f, k;

            if (i < 20) {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            } else if (i < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

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
    ss << hex << uppercase << setfill('0');
    ss << setw(8) << h0
       << setw(8) << h1
       << setw(8) << h2
       << setw(8) << h3
       << setw(8) << h4;

    return ss.str();
}

int main() {
    string input;
    cout << "Enter input string: ";
    getline(cin, input);

    string hash = sha1(input);
    cout << "SHA-1 Hash: " << hash << endl;

    return 0;
}
