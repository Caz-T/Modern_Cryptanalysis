//
// Created by Chen Qingzhi on 2023/5/20.
//

#include "sha256.h"
#include <memory>

#define idx(x, y) (5 * (y) + (x))

void id_map(uint64_t* state) {
    // for temporary storage
    auto prime = new uint64_t[STATE_SIZE / 64];
    memset(prime, 0, STATE_SIZE);

    for (uint64_t ir = 12 + 2 * LANE_LOG - 24; ir < 12 + 2 * LANE_LOG; ir++) {
        uint64_t x, y, z, t;

        // θ
        auto c = new uint64_t[5], d = new uint64_t[5];
        memset(c, 0, 5 * 64);
        memset(d, 0, 5 * 64);
        for (x = 0; x < 5; x++)
            for (t = 0 ; t < 5; t++)
                c[x] ^= state[idx(x, t)];
        for (x = 0; x < 5; x++)
            d[x] = c[(x + 4) % 5] ^ cycl(c[(x + 1) % 5], 1);
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                state[idx(x, y)] ^= d[x];
        delete[] c;
        delete[] d;

        // ρ
        x = 1;
        y = 0;
        prime[idx(0, 0)] = state[idx(0, 0)];
        for (t = 0; t < 24; t++) {
            prime[idx(x, y)] = cycl(state[idx(x, y)], (t + 1) * (t + 2) / 2);
            uint64_t temp = y;
            y = (2 * x + 3 * y) % 5;
            x = temp;
        }
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                state[idx(x, y)] = prime[idx(x, y)];

        // π
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                prime[idx(x, y)] = state[idx((x + 3 * y) % 5, x)];
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                state[idx(x, y)] = prime[idx(x, y)];

        // χ
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                prime[idx(x, y)] = state[idx(x, y)] ^ (~state[idx((x + 1) % 5, y)] & state[idx((x + 2) % 5, y)]);
        for (x = 0; x < 5; x++)
            for (y = 0; y < 5; y++)
                state[idx(x, y)] = prime[idx(x, y)];

        // ι
        uint64_t rc = 0ull;
        for (t = 0; t <= LANE_LOG; t++) {
            if (RC_LOOKUP[t + 7 * ir]) setbit(rc, (1ull << t) - 1);
            else resetbit(rc, (1ull << t) - 1);
        }
        state[idx(0, 0)] ^= rc;
    }
    delete[] prime;
}

u_int8_t* sha256::hash(uint64_t* input, uint64_t length) {
    // divide into blocks of equal length
    auto* curr_state = new uint64_t[STATE_SIZE / 64];
    // void suffix (c-part)
    for (uint64_t i = CONSTANT_R / 64; i < STATE_SIZE / 64; i++) curr_state[i] = 0;

    for (uint64_t i = 0; i < length / CONSTANT_R; i++) {
        // absorption
        for (uint64_t j = 0; j < CONSTANT_R / 64; j++) curr_state[j] ^= input[i * CONSTANT_R / 64 + j];
        id_map(curr_state);
    }

    // truncating
    return (uint8_t*)curr_state;
}
