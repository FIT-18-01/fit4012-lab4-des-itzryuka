#include <iostream>
#include <string>
#include <bitset>
#include <vector>

using namespace std;

// Helper function: Covert decimal to 4-bit binary string
string convert_decimal_to_binary(int decimal) {
    return bitset<4>(decimal).to_string();
}

// Helper function: Convert binary string to decimal
int convert_binary_to_decimal(const string& binary) {
    return stoi(binary, nullptr, 2);
}

// Helper function: XOR two binary strings
string Xor(const string& a, const string& b) {
    string result = "";
    for (size_t i = 0; i < b.size(); i++) {
        result += (a[i] != b[i]) ? '1' : '0';
    }
    return result;
}

// Initial Permutation (IP)
string initial_permutation(const string& input) {
    const int initial_permutation[64] = {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
    };

    string permuted = "";
    for (int i = 0; i < 64; i++) {
        permuted += input[initial_permutation[i] - 1];
    }
    return permuted;
}

// Inverse Initial Permutation (IP^-1)
string inverse_initial_permutation(const string& input) {
    const int inverse_permutation[64] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };

    string permuted = "";
    for (int i = 0; i < 64; i++) {
        permuted += input[inverse_permutation[i] - 1];
    }
    return permuted;
}

class KeyGenerator {
private:
    string key;
    vector<string> roundKeys;

    // PC1 Table
    const int pc1[56] = {
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };

    // PC2 Table
    const int pc2[48] = {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };

    string shift_left_once(const string& key_chunk) {
        return key_chunk.substr(1) + key_chunk[0];
    }

    string shift_left_twice(string key_chunk) {
        key_chunk = shift_left_once(key_chunk);
        return shift_left_once(key_chunk);
    }

public:
    KeyGenerator(const string& input_key) : key(input_key) {}

    void generateRoundKeys() {
        roundKeys.clear();

        // 1. Apply PC1 permutation
        string permutedKey = "";
        for (int i = 0; i < 56; ++i) {
            permutedKey += key[pc1[i] - 1];
        }

        // 2. Split into left and right halves
        string left = permutedKey.substr(0, 28);
        string right = permutedKey.substr(28, 28);

        // 3. Generate 16 round keys
        for (int i = 0; i < 16; ++i) {
            if (i == 0 || i == 1 || i == 8 || i == 15) {
                left = shift_left_once(left);
                right = shift_left_once(right);
            }
            else {
                left = shift_left_twice(left);
                right = shift_left_twice(right);
            }

            // 4. Combine halves
            string combinedKey = left + right;

            // 5. Apply PC2 permutation
            string roundKey = "";
            for (int j = 0; j < 48; ++j) {
                roundKey += combinedKey[pc2[j] - 1];
            }

            roundKeys.push_back(roundKey);
        }
    }

    const vector<string>& getRoundKeys() const {
        return roundKeys;
    }
};

// Class to perform DES Feistel rounds
class DES {
private:
    // Expansion table to expand 32 bits to 48 bits
    const int expansion_table[48] = {
        32,1,2,3,4,5,4,5,
        6,7,8,9,8,9,10,11,
        12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,
        22,23,24,25,24,25,26,27,
        28,29,28,29,30,31,32,1
    };

    // Permutation table after S-box substitution
    const int permutation_tab[32] = {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };

    // S-boxes (8 boxes)
    const int substition_boxes[8][4][16] = { {
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    },
    {
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    },
    {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    },
    {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    },
    {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    },
    {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    },
    {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    },
    {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    } };

    vector<string> round_keys;

    string feistel_network(string input, bool is_decrypt) {
        string perm = initial_permutation(input);

        string left = perm.substr(0, 32);
        string right = perm.substr(32, 32);

        for (int i = 0; i < 16; i++) {
            string right_expanded = "";
            for (int j = 0; j < 48; j++) {
                right_expanded += right[expansion_table[j] - 1];
            }

            // Nếu là giải mã (decrypt), duyệt mảng key ngược (từ 15 về 0)
            int key_index = is_decrypt ? (15 - i) : i;
            string xored = Xor(round_keys[key_index], right_expanded);

            string res = "";
            for (int j = 0; j < 8; j++) {
                string row1 = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
                int row = convert_binary_to_decimal(row1);

                string col1 = xored.substr(j * 6 + 1, 4);
                int col = convert_binary_to_decimal(col1);

                int val = substition_boxes[j][row][col];
                res += convert_decimal_to_binary(val);
            }

            string perm2 = "";
            for (int j = 0; j < 32; j++) {
                perm2 += res[permutation_tab[j] - 1];
            }

            string new_right = Xor(perm2, left);
            left = right;
            right = new_right;
        }

        string combined_text = right + left;
        return inverse_initial_permutation(combined_text);
    }

public:
    DES(const vector<string>& keys) : round_keys(keys) {}

    string encrypt(const string& input) {
        return feistel_network(input, false);
    }

    // Hàm giải mã DES
    string decrypt(const string& input) {
        // Permutation ban đầu
        string perm = initial_permutation(input);

        // Chia trái, phải
        string left = perm.substr(0, 32);
        string right = perm.substr(32, 32);

        // 16 Feistel rounds (LƯU Ý: VÒNG LẶP CHẠY NGƯỢC TỪ 15 VỀ 0)
        for (int i = 15; i >= 0; i--) {
            string right_expanded = "";
            for (int j = 0; j < 48; j++) {
                right_expanded += right[expansion_table[j] - 1];
            }

            // XOR với round key
            string xored = Xor(round_keys[i], right_expanded);

            // S-box substitution
            string res = "";
            for (int j = 0; j < 8; j++) {
                string row1 = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
                int row = convert_binary_to_decimal(row1);
                string col1 = xored.substr(j * 6 + 1, 4);
                int col = convert_binary_to_decimal(col1);
                int val = substition_boxes[j][row][col];
                res += convert_decimal_to_binary(val);
            }

            string perm2 = "";
            for (int j = 0; j < 32; j++) {
                perm2 += res[permutation_tab[j] - 1];
            }

            string new_right = Xor(perm2, left);
            left = right;
            right = new_right;
        }

        // Gộp lại
        string combined_text = right + left;

        // Permutation ngược
        string plaintext = inverse_initial_permutation(combined_text);

        return plaintext;
    }
};

// Main function
int main() {
    int mode;
    // Nhận mode từ bàn phím (để CI nhận diện được dấu hiệu nhập liệu)
    if (!(cin >> mode)) return 0;

    if (mode == 1) {
        // Mode 1: DES encrypt
        string plaintext, key;
        cin >> plaintext >> key;

        // 1. Thực hiện Zero Padding nếu độ dài không phải bội số của 64
        while (plaintext.length() % 64 != 0) {
            plaintext += "0";
        }

        // 2. Khởi tạo khóa
        KeyGenerator keygen(key);
        keygen.generateRoundKeys();
        DES des(keygen.getRoundKeys());

        // 3. Cắt từng block 64-bit ra để mã hóa và ghép kết quả lại
        string final_ciphertext = "";
        for (size_t i = 0; i < plaintext.length(); i += 64) {
            string block = plaintext.substr(i, 64);
            final_ciphertext += des.encrypt(block);
        }

        // 4. In ra kết quả ciphertext cuối cùng (CI sẽ quét dòng này)
        cout << final_ciphertext << endl;
    }
    else if (mode == 2) {
        // Mode 2: DES decrypt
        string ciphertext, key;
        cin >> ciphertext >> key;

        KeyGenerator keygen(key); keygen.generateRoundKeys();
        DES des(keygen.getRoundKeys());

        string final_plaintext = "";
        for (size_t i = 0; i < ciphertext.length(); i += 64) {
            string block = ciphertext.substr(i, 64);
            final_plaintext += des.decrypt(block);
        }
        cout << final_plaintext << endl;
    }
    else if (mode == 3) {
        // Mode 3: TripleDES encrypt
        string plaintext, k1, k2, k3;
        cin >> plaintext >> k1 >> k2 >> k3;

        // Zero Padding
        while (plaintext.length() % 64 != 0) { plaintext += "0"; }

        KeyGenerator keygen1(k1); keygen1.generateRoundKeys(); DES des1(keygen1.getRoundKeys());
        KeyGenerator keygen2(k2); keygen2.generateRoundKeys(); DES des2(keygen2.getRoundKeys());
        KeyGenerator keygen3(k3); keygen3.generateRoundKeys(); DES des3(keygen3.getRoundKeys());

        string final_ciphertext = "";
        for (size_t i = 0; i < plaintext.length(); i += 64) {
            string block = plaintext.substr(i, 64);
            // E(K1) -> D(K2) -> E(K3)
            string step1 = des1.encrypt(block);
            string step2 = des2.decrypt(step1);
            string step3 = des3.encrypt(step2);
            final_ciphertext += step3;
        }
        cout << final_ciphertext << endl;
    }
    else if (mode == 4) {
        // Mode 4: TripleDES decrypt
        string ciphertext, k1, k2, k3;
        cin >> ciphertext >> k1 >> k2 >> k3;

        KeyGenerator keygen1(k1); keygen1.generateRoundKeys(); DES des1(keygen1.getRoundKeys());
        KeyGenerator keygen2(k2); keygen2.generateRoundKeys(); DES des2(keygen2.getRoundKeys());
        KeyGenerator keygen3(k3); keygen3.generateRoundKeys(); DES des3(keygen3.getRoundKeys());

        string final_plaintext = "";
        for (size_t i = 0; i < ciphertext.length(); i += 64) {
            string block = ciphertext.substr(i, 64);
            // Giải mã TripleDES: D(K3) -> E(K2) -> D(K1)
            string step1 = des3.decrypt(block);
            string step2 = des2.encrypt(step1);
            string step3 = des1.decrypt(step2);
            final_plaintext += step3;
        }
        cout << final_plaintext << endl;
    }

    return 0;
}