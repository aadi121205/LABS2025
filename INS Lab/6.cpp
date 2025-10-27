#include <iostream>
#include <string>
#include <algorithm> // For std::rotate, a simple way to do circular shifts

using namespace std;

// --- Helper Functions ---

/**
 * @brief Applies a permutation table to a key.
 * @param key The input key string.
 * @param p The permutation table (array of integers).
 * @param n The size of the permutation table.
 * @return The permuted key string.
 */
string permute(string key, int* p, int n) {
    string permuted_key = "";
    for (int i = 0; i < n; i++) {
        // P-box indices are 1-based, array indices are 0-based
        permuted_key += key[p[i] - 1];
    }
    return permuted_key;
}

/**
 * @brief Performs a circular left shift on a 5-bit string.
 * @param key The 5-bit key half.
 * @param shifts The number of positions to shift.
 * @return The shifted string.
 */
string left_shift(string key_half, int shifts) {
    string shifted = key_half;
    // std::rotate performs a circular left shift
    // rotate(begin, middle, end)
    // The element at 'middle' becomes the new first element
    rotate(shifted.begin(), shifted.begin() + shifts, shifted.end());
    return shifted;
}

// --- Main S-DES Key Generation Logic ---

int main() {
    // --- Define S-DES Parameters ---

    // P10 (Permutation 10) table
    // Input: (1 2 3 4 5 6 7 8 9 10)
    // Output: (3 5 2 7 4 10 1 9 8 6)
    int p10_table[] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };

    // P8 (Permutation 8) table
    // Input: (1 2 3 4 5 6 7 8 9 10) <- from the 10-bit shifted key
    // Output: (6 3 7 4 8 5 10 9)
    int p8_table[] = { 6, 3, 7, 4, 8, 5, 10, 9 };

    // --- Key Generation Steps ---

    string key_10bit;
    string k1, k2;

    // 1. Get 10-bit key from user
    cout << "Enter a 10-bit key (e.g., 1010000010): ";
    cin >> key_10bit;

    if (key_10bit.length() != 10) {
        cout << "Error: Key must be 10 bits long." << endl;
        return 1;
    }

    // 2. Apply P10
    string p10_key = permute(key_10bit, p10_table, 10);
    cout << "After P10:       " << p10_key << endl;

    // 3. Split into 5-bit halves
    string left_half = p10_key.substr(0, 5);
    string right_half = p10_key.substr(5, 5);
    cout << "Left half:       " << left_half << endl;
    cout << "Right half:      " << right_half << endl;
    cout << "---------------------------------" << endl;


    // --- Generate K1 ---
    
    // 4. Apply LS-1 (Circular Left Shift by 1)
    string ls1_left = left_shift(left_half, 1);
    string ls1_right = left_shift(right_half, 1);
    cout << "After LS-1 Left:  " << ls1_left << endl;
    cout << "After LS-1 Right: " << ls1_right << endl;

    // 5. Combine halves and apply P8 to get K1
    string combined_k1 = ls1_left + ls1_right;
    cout << "Combined for K1: " << combined_k1 << endl;
    k1 = permute(combined_k1, p8_table, 8);
    
    cout << "---------------------------------" << endl;


    // --- Generate K2 ---

    // 6. Apply LS-2 (Circular Left Shift by 2) to the *already shifted* halves (ls1)
    string ls2_left = left_shift(ls1_left, 2);
    string ls2_right = left_shift(ls1_right, 2);
    cout << "After LS-2 Left:  " << ls2_left << endl;
    cout << "After LS-2 Right: " << ls2_right << endl;

    // 7. Combine halves and apply P8 to get K2
    string combined_k2 = ls2_left + ls2_right;
    cout << "Combined for K2: " << combined_k2 << endl;
    k2 = permute(combined_k2, p8_table, 8);


    // --- Final Output ---
    cout << "=================================" << endl;
    cout << "Initial 10-bit Key: " << key_10bit << endl;
    cout << "Subkey K1:          " << k1 << endl;
    cout << "Subkey K2:          " << k2 << endl;
    cout << "=================================" << endl;

    return 0;
}