#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "sha256.h"
#include "constants.h"
using namespace std;


int main(int argc, char* argv[]) {
    ifstream fi;
    fi.open(argc == 1 ? "to_hash.txt" : argv[1]);
    string temp;
    fi >> temp;
    fi.close();
    temp += "011";
    while (temp.length() % CONSTANT_R != CONSTANT_R - 1) temp += "0";
    temp += "1";
    auto input = new uint64_t[temp.length() / 64];
    for (uint64_t i = 0; i < temp.length() / 64; i++) input[i] = 0ull;
    for (uint64_t i = 0; i < temp.length(); i++)
        if (temp[i] == '1') setbit(input[i / 64], i % 64);
    cout << "Start hashing..." << endl;
    auto init = duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()
    ).count();
    auto result = sha256::hash(input, temp.length());
    auto fin = duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()
    ).count();
    cout << "Hashing finished in " << fin - init << " ms" << endl;

    ofstream fo;
    fo.open("hash_result.txt");
    for (uint64_t i = 0; i < 32; i++) fo << hex << setw(2) << setfill('0') << (unsigned short)result[i] << " ";
    fo.close();
    return 0;
}
