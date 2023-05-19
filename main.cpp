#include <iostream>
#include <string>

#include "aes.h"
using namespace std;


int main() {
    // unsigned char* key = nullptr;
    auto key = new unsigned char[16];
    for (int i = 0; i < 16; i++) key[i] = (unsigned char)i;
    auto machine = new aes(key);
    string to_encrypt = "Shall I compare thee to a summer's day / Thou art more lovely and more temperate";
    auto result = machine->encrypt(to_encrypt, &key);
    auto back = machine->decrypt(result, 80, key);
    for (int i = 0; i < 16; i++) cout << hex << (unsigned short)key[i] << " ";
    cout << endl;
    for (int i = 0; i < to_encrypt.length(); i++) cout << hex << (unsigned short)result[i] << " ";
    cout << endl << back << endl;
    return 0;
}
