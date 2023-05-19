//
// Created by Chen Qingzhi on 2023/5/18.
//

#include <vector>
#include <random>
#include "aes.h"
#include "constants.h"
using namespace std;

unsigned char* aes::encrypt(string& original_text, unsigned char** iv) {
    // group by 16 bytes
    vector<block*> ans;
    unsigned long long len = original_text.length();
    // generate initial vector, using CBC
    // cited from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 255);
    *iv = new unsigned char[16];
    for (int i = 0; i < 16; i++) *iv[i] = (unsigned char)distr(gen);

    for (unsigned long long i = 0; 16 * i < len; i++) {
        auto transcripted = new unsigned char[16];
        for (unsigned long long j = 0; j < 16 and j < len; j++) transcripted[j] = original_text[16 * i + j];
        // padding
        for (unsigned long long j = len; j < 16 * (i + 1); j++) transcripted[j] = 0;
        auto curr_state = new block(transcripted);

        // populate round keys
        block* round_keys[11];
        if (i == 0) round_keys[0] = new block(*iv);  // for the first block use iv as the first round_key
        else round_keys[0] = new block(*ans.back()); // else use the last encrypted block
        auto w = new unsigned int[44];  // in slides' fashion
        for (int j = 0; j < 4; j++) w[j] = *iv[j];
        for (int j = 4; j < 44; j++) {
            auto temp = w[j - 1];
            if (j % 4 == 0) temp = S_BOX[(temp << 2) + (temp >> 6)] ^ RCON[(int)(j / 4)];
            w[j] = w[j - 4] ^ temp;
        }

        // CBC implementation
        curr_state->xor_with(*round_keys[0]);
        for (int round = 0; round < 9; round++) curr_state->step_through(*round_keys[round]);
        curr_state->step_through(*round_keys[10], false);

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

string aes::decrypt(unsigned char* coded_text) {
    string ans;
    return ans;
}
