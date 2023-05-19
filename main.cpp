#include <iostream>
#include <string>

#include "aes.h"
using namespace std;

const unsigned char TEST_1[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c,
};

int main() {
    // unsigned char* key = nullptr;
    auto iv_key = new unsigned char[16];
    auto custom_key = new unsigned char[16];
    strcpy(reinterpret_cast<char *>(custom_key), "I render it pure.");

    for (int i = 0; i < 16; i++) {
        iv_key[i] = (unsigned char)i;
        custom_key[i] = TEST_1[i];
    }


    auto machine = new aes(custom_key);
    string to_encrypt = "Shall I compare thee to a summer's day / Thou art more lovely and more temperate";
    auto result = machine->encrypt(to_encrypt, &iv_key);
    auto back = machine->decrypt(result, 80, iv_key);
    for (int i = 0; i < 16; i++) cout << hex << (unsigned short)iv_key[i] << " ";
    cout << endl;
    for (int i = 0; i < to_encrypt.length(); i++) cout << hex << (unsigned short)result[i] << " ";
    cout << endl << back << endl;
    return 0;
}
