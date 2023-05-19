//
// Created by Chen Qingzhi on 2023/5/18.
//

#include <vector>
#include <random>
#include "aes.h"
#include "constants.h"
using namespace std;

aes::aes(unsigned char* ck) {
    cypher_key = ck;
    round_keys[0] = new block(ck);
    auto w = new unsigned int[44];  // in slides' fashion
    for (int j = 0; j < 4; j++) w[j] = (ck[4 * j] << 24) + (ck[4 * j + 1] << 16) + (ck[4 * j + 2] << 8) + (ck[4 * j + 3]);
    for (int j = 4; j < 44; j++) {
        unsigned int temp = w[j - 1];
        if (j % 4 == 0) {
            temp = (unsigned int)S_BOX[temp >> 24] + (unsigned int)(S_BOX[temp & 0xFF] << 8)
                   + (unsigned int)(S_BOX[(temp >> 8) & 0xFF] << 16) + (unsigned int)(S_BOX[(temp >> 16) & 0xFF] << 24);
            temp = temp ^ RCON[(int)(j / 4)];
        }
        w[j] = w[j - 4] ^ temp;
    }
    for (int j = 1; j <= 10; j++) {
        auto temp = new unsigned char[16];
        for (int k = 0; k < 4; k++)
            for (int l = 0; l < 4; l++)
                temp[k * 4 + l] = w[j * 4 + k] >> (8 * (3 - l)) & 0xFF;
        round_keys[j] = new block(temp);
    }
}

unsigned char* aes::encrypt(string& original_text, unsigned char** iv) {
    // group by 16 bytes
    vector<block*> ans;
    unsigned long long len = original_text.length();
    // generate initial vector, using CBC
    // cited from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    if (*iv == nullptr) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0, 255);
        *iv = new unsigned char[16];
        for (int i = 0; i < 16; i++) (*iv)[i] = (unsigned char)distr(gen);
    }

    for (unsigned long long i = 0; 16 * i < len; i++) {
        auto transcripted = new unsigned char[16];
        for (unsigned long long j = 0; j < 16 and j < len; j++) transcripted[j] = original_text[16 * i + j];
        // padding
        for (unsigned long long j = len; j < 16 * (i + 1); j++) transcripted[j] = 0;
        auto curr_state = new block(transcripted);

        // CBC overhead
        if (i == 0) {
            auto iv_block = new block(*iv);
            curr_state->xor_with(iv_block);
            delete iv_block;
        } else curr_state->xor_with(ans.back());
        // AES part
        curr_state->xor_with(round_keys[0]);
        for (int round = 0; round < 9; round++) curr_state->step_through(round_keys[round]);
        curr_state->step_through(round_keys[10], false);

        // save to vector `ans`
        ans.emplace_back(curr_state);
    }

    // decode to output format (hex)
    auto to_ret = new unsigned char[ans.size() * 16];
    for (int i = 0; i < ans.size(); i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                to_ret[16 * i + 4 * j + k] = ans[i]->bytes[j][k];
    return to_ret;
}

string aes::decrypt(unsigned char* coded_text, unsigned long long len, unsigned char* iv) {
    vector<block*> ans;
    for (unsigned long long i = 0; 16 * i < len; i++) {
        auto curr_state = new block(&coded_text[16 * i]);

        // AES decypher
        curr_state->xor_with(round_keys[10]);
        for (int round = 9; round >= 0; round--) curr_state->step_back(round_keys[round]);
        curr_state->step_through(round_keys[0], false);
        // CBC overhead
        if (i == 0) {
            auto iv_block = new block(iv);
            curr_state->xor_with(iv_block);
            delete iv_block;
        } else curr_state->xor_with(ans.back());

        // save to vector `ans`
        ans.emplace_back(curr_state);
    }

    // decode to output format (string)
    string to_ret;
    for (auto & an : ans)
        for (auto & byte : an->bytes)
            for (unsigned char k : byte)
                to_ret += char(k);
    return to_ret;
}
