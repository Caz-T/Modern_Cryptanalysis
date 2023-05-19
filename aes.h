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
    static unsigned char* encrypt(string&, unsigned char**);
    static string decrypt(unsigned char*);
};


#endif //AES128_CBC_AES_H
