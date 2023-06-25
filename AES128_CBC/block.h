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

    void step_through(const block*, bool mix_col = true);
    void step_back(const block*, bool inv_mix_col = true);
    void xor_with(const block*);


// private:
    void substitute_bytes();
    void shift_rows();
    void mix_columns();
    void inv_substitute_bytes();
    void inv_shift_rows();
    void inv_mix_columns();
};


#endif //AES128_CBC_BLOCK_H
