//
// Created by Chen Qingzhi on 2023/5/20.
//

#ifndef SHA256_CONSTANTS_H
#define SHA256_CONSTANTS_H

// The o-th bit, LSB first
#define getbit(u, o) (((u) >> (o)) & 0x1ull)
#define setbit(u, o) ((u) = ((u) | (0x1ull << (o))))
#define resetbit(u, o) ((u) = ((u) & (0x1ull << (o))))

#define cycl(u, num) ((u) << (num) | (u) >> (64 - (num)))
#define cycr(u, num) ((u) >> (num) | (u) << (64 - (num)))

const uint32_t STATE_SIZE = 1600;
const uint32_t CAPACITY = 512;
const uint32_t CONSTANT_R = STATE_SIZE - CAPACITY;
const uint32_t OUTPUT_LEN = 256;
const uint32_t ROUND_COUNT = 24;
const uint32_t LANE_LEN = STATE_SIZE / 25;
const uint32_t LANE_LOG = 6;  // log_2 (STATE_SIZE / 25)

const uint32_t MAX_INPUT_SIZE = 1073741823; // maximum input size: 125MB (1G bits)


// self-computed.
/*
bool rc(uint64_t t) {
    if (t % 255 == 0) return true;
    uint8_t r = 0x80;
    for (uint64_t i = 0; i < t % 255; i++) {
        uint8_t mask = r % 2;
        r >>= 1;
        r ^= (mask << 7);
        r ^= (mask << 3);
        r ^= (mask << 2);
        r ^= (mask << 1);
    }
    return r >> 7;
}
int main() {
    for (uint64_t t = 0; t < 16; t++) {
        for (uint64_t v = 0; v < 16; v++) {
            if (rc(t * 16 + v)) cout << "true, ";
            else cout << "false, ";
        }
        cout << endl;
    }
}
 */
const bool RC_LOOKUP[256] = {
        true, true, true, false, true, false, false, false, false, true, true, true, true, true, true, true,
        true, false, false, true, false, false, false, false, true, false, true, false, false, true, true, true,
        true, true, false, true, false, true, false, true, false, true, true, true, false, false, false, false,
        false, true, true, false, false, false, true, false, true, false, true, true, false, false, true, true,
        false, false, true, false, true, true, true, true, true, true, false, true, true, true, true, false,
        false, true, true, false, true, true, true, false, true, true, true, false, false, true, false, true,
        false, true, false, false, true, false, true, false, false, false, true, false, false, true, false, true,
        true, false, true, false, false, false, true, true, false, false, true, true, true, false, false, true,
        true, true, true, false, false, false, true, true, false, true, true, false, false, false, false, true,
        false, false, false, true, false, true, true, true, false, true, false, true, true, true, true, false,
        true, true, false, true, true, true, true, true, false, false, false, false, true, true, false, true,
        false, false, true, true, false, true, false, true, true, false, true, true, false, true, false, true,
        false, false, false, false, false, true, false, false, true, true, true, false, true, true, false, false,
        true, false, false, true, false, false, true, true, false, false, false, false, false, false, true, true,
        true, false, true, false, false, true, false, false, false, true, true, true, false, false, false, true,
        };

#endif //SHA256_CONSTANTS_H
