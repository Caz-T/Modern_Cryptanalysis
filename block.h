//
// Created by Chen Qingzhi on 2023/5/18.
//

#ifndef AES128_CBC_BLOCK_H
#define AES128_CBC_BLOCK_H


class block {
public:
    unsigned char bytes[4][4];

    block();
    explicit block(const unsigned char*);

    void step_through(const block&);
    void xor_with(const block& another);
};


#endif //AES128_CBC_BLOCK_H
