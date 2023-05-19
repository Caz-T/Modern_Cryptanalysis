//
// Created by Chen Qingzhi on 2023/5/18.
//

#ifndef AES128_CBC_AES_H
#define AES128_CBC_AES_H

#include <string>
#include "block.h"
#include "constants.h"
using namespace std;

class aes {
public:
    explicit aes(unsigned char*);
    unsigned char* encrypt(string&, unsigned char**);
    string decrypt(unsigned char*, unsigned long long, unsigned char*);

private:
    block* round_keys[11];
};


#endif //AES128_CBC_AES_H
