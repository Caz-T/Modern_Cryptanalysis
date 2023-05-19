//
// Created by Chen Qingzhi on 2023/5/18.
//

#include <memory>
#include "block.h"
#include "constants.h"
using namespace std;

block::block() {
    memset(bytes, 0, 16 * sizeof(unsigned char));
}

block::block(const unsigned char * ori) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] = ori[i];
}

void block::xor_with(const block &another) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] ^= another.bytes[i][j];
}

void block::substitute_bytes() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] = S_BOX[bytes[i][j]];
}

void block::shift_rows() {
    for (int i = 1; i < 4; i++) {
        unsigned char temp = bytes[i][i];
        for (int j = 0; j < 3; j++)
            bytes[i][(j + i) % 4] = bytes[i][(j + i * 2) % 4];
        bytes[i][(i + 3) % 4] = temp;
    }
}

void block::mix_columns() {
    auto oldbyte = new block();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                oldbyte->bytes[i][j] += GALOIS_TABLE[MIX_MATRIX[i][k]][bytes[k][j]];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] = oldbyte->bytes[i][j];
    delete oldbyte;
}

void block::step_through(const block& round_key, bool mix_col) {
    substitute_bytes();
    shift_rows();
    if (mix_col) mix_columns();
    xor_with(round_key);
}
