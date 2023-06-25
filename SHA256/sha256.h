//
// Created by Chen Qingzhi on 2023/5/20.
//
// ref: https://nicodechal.github.io/2019/04/08/hash-funciton-sha3/

#ifndef SHA256_SHA256_H
#define SHA256_SHA256_H

#include <cstdlib>
#include "constants.h"

using namespace std;

class sha256 {
public:
    static u_int8_t* hash(uint64_t* input, uint64_t length);
    // length is in bits, padded

};


#endif //SHA256_SHA256_H
