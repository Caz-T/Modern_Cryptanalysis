#include <iostream>
#include <string>

#include "aes.h"
using namespace std;


int main() {
    unsigned char* key;
    string to_encrypt = "Shall I compare thee to a summer's day / Thou art more lovely and more temperate";
    auto result = aes::encrypt(to_encrypt, &key);
    for (int i = 0; i < 16; i++) cout << hex << (unsigned short)key[i] << " ";
    cout << endl;
    for (int i = 0; i < to_encrypt.length(); i++) cout << hex << (unsigned short)result[i] << " ";
    return 0;
}
