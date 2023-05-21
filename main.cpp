#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "sha256.h"
#include "constants.h"
using namespace std;


int main() {
    ifstream fi;
    fi.open("to_hash.txt");
    string temp;
    fi >> temp;
    fi.close();
    temp += "011";
    while (temp.length() % CONSTANT_R != CONSTANT_R - 1) temp += "0";
    temp += "1";
    auto input = new uint64_t[temp.length() / 64];
    for (uint64_t i = 0; i < temp.length(); i++)
        if (temp[i] == '1') setbit(input[i / 64], i % 64);
    cout << "here";
    auto
    auto result = sha256::hash(input, temp.length());

    ofstream fo;
    fo.open("hash_result.txt");
    for (uint64_t i = 0; i < 32; i++) fo << hex << setw(2) << setfill(' ') << (unsigned short)result[i] << " ";
    fo.close();
    return 0;
}
