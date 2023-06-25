#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

#include "aes.h"
using namespace std;

const unsigned char STD_KEY[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
const unsigned char TEST_1[16] = {
        0x6a, 0x6a, 0x5c, 0x45, 0x2c, 0x6d, 0x33, 0x51, 0xb0, 0xd9, 0x5d, 0x61, 0x27, 0x9c, 0x21, 0x5c
};

int main() {
    auto custom_key = new unsigned char[16];
    for (int i = 0; i < 16; i++) custom_key[i] = STD_KEY[i];
    auto machine = new aes(custom_key);

    cout << "Choose one: [d]ecrypt / [e]ncrypt :";
    char c;
    cin >> c;
    if (c == 'e') {
        // Read input from file
        // string to_encrypt = "Shall I compare thee to a summer's day / Thou art more lovely and more temperate:";
        string to_encrypt;
        ifstream fi;
        fi.open("to_encrypt.txt");
        getline(fi, to_encrypt);
        fi.close();
        auto len = to_encrypt.length();

        // generate initial vector, using CBC
        // cited from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
        unsigned char* iv_key = new unsigned char[16];
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(1, 255);
        ofstream keyfile;
        keyfile.open("iv_key.txt");
        for (int i = 0; i < 16; i++) {
            iv_key[i] = (unsigned char)distr(gen);
            keyfile << hex << setfill('0') << setw(2) << (unsigned short)iv_key[i] << " ";
        }
        keyfile.close();

        auto initial_time = duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()
        ).count();
        auto result = machine->encrypt(to_encrypt, iv_key);
        auto fin = duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()
        ).count();
        cout << "Encryption time: " << fin - initial_time << endl;

        ofstream fo;
        fo.open("encrypt_result.txt");
        for (auto i = 0; i < ((unsigned long long)(len / 16)) * 16; i++) fo << hex << setfill('0') << setw(2) << (unsigned short)result[i] << ' ';
        fo.close();
    } else if (c == 'd') {
        ifstream fi;
        fi.open("to_decrypt.txt");
        unsigned long long len = 0;
        unsigned short t;
        auto to_decrypt = new unsigned char [1073741823];
        while (fi >> hex >> t) {
            to_decrypt[len] = t;
            len += 1;
        }

        // get iv from file
        ifstream fi_iv;
        fi_iv.open("iv_key.txt");
        auto iv_key = new unsigned char[16];
        for (int i = 0; i < 16; i++) {
            fi_iv >> hex >> t;
            iv_key[i] = t;
        }
        auto init_time = duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()
        ).count();
        auto back = machine->decrypt(to_decrypt, len, iv_key);
        auto fin = duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()
        ).count();
        cout << "Decryption time: " << fin - init_time << endl;

        // write to file
        ofstream result;
        result.open("decrypt_result.txt");
        result << back;
        result.close();
    }
    return 0;
}
