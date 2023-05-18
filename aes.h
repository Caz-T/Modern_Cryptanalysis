//
// Created by Chen Qingzhi on 2023/5/18.
//

#ifndef AES128_CBC_AES_H
#define AES128_CBC_AES_H

#include <string>
using namespace std;

class aes {
public:
    explicit aes(string key);
    string cypher(string original_text);
    string decypher(string coded_text);

private:
    string key;
};


#endif //AES128_CBC_AES_H
