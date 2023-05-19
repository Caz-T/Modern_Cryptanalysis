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
            bytes[i][j] = ori[j * 4 + i];
}

void block::xor_with(const block* another) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] ^= another->bytes[i][j];
}

void block::substitute_bytes() {
    for (auto & byte : bytes)
        for (unsigned char & j : byte)
            j = S_BOX[j];
}

void block::shift_rows() {
    // We're doing this manually to maximise efficiency
    unsigned char temp1, temp2;
    temp1 = bytes[1][0];
    for (int j = 0; j < 3; j++) bytes[1][j] = bytes[1][j + 1];
    bytes[1][3] = temp1;
    temp1 = bytes[2][0];
    temp2 = bytes[2][1];
    bytes[2][0] = bytes[2][2];
    bytes[2][1] = bytes[2][3];
    bytes[2][2] = temp1;
    bytes[2][3] = temp2;
    temp1 = bytes[3][3];
    for (int j = 3; j > 0; j--) bytes[3][j] = bytes[3][j - 1];
    bytes[3][0] = temp1;
}

void block::mix_columns() {
    auto oldbyte = new block();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                oldbyte->bytes[i][j] ^= GALOIS_TABLE[MIX_MATRIX[i][k]][bytes[k][j]];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] = oldbyte->bytes[i][j];
    delete oldbyte;
}

void block::step_through(const block* round_key, bool mix_col) {
    substitute_bytes();
    shift_rows();
    if (mix_col) mix_columns();
    xor_with(round_key);
}

void block::inv_substitute_bytes() {
    for (auto & byte : bytes)
        for (unsigned char & j : byte)
            j = INVERSE_S_BOX[j];
}

void block::inv_shift_rows() {
    unsigned char temp1, temp2;
    temp1 = bytes[3][0];
    for (int j = 0; j < 3; j++) bytes[3][j] = bytes[3][j + 1];
    bytes[3][3] = temp1;
    temp1 = bytes[2][0];
    temp2 = bytes[2][1];
    bytes[2][0] = bytes[2][2];
    bytes[2][1] = bytes[2][3];
    bytes[2][2] = temp1;
    bytes[2][3] = temp2;
    temp1 = bytes[1][3];
    for (int j = 3; j > 0; j--) bytes[1][j] = bytes[1][j - 1];
    bytes[1][0] = temp1;
}

void block::inv_mix_columns() {
    auto oldbyte = new block();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                oldbyte->bytes[i][j] ^= GALOIS_TABLE[INVERSE_MIX_MATRIX[i][k]][bytes[k][j]];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bytes[i][j] = oldbyte->bytes[i][j];
    delete oldbyte;
}

void block::step_back(const block* round_key, bool inv_mix_col) {
    inv_substitute_bytes();
    inv_shift_rows();
    xor_with(round_key);
    if (inv_mix_col) inv_mix_columns();
}