#include <iostream>
#include <gmpxx.h>
#include <random>
#include <string>

using namespace std;

random_device rd;
mt19937 gen(rd());
gmp_randstate_t randstate;

const int RSA_BITS = 2048;
const mpz_class MPZ_TWO = 2;
const int SMALL_PRIMES[10] = {
        2, 3, 5, 7, 11, 13, 17, 23, 29, 31
};

bool miller_rabin(mpz_class& n, int round_count = 40) {
    // default value set to 40 according to https://security.stackexchange.com/questions/4544/how-many-iterations-of-rabin-miller-should-be-used-to-generate-cryptographic-saf
    for (int i : SMALL_PRIMES) if (n % i == 0) return n == i;

    mpz_class k = 0, m = n - 1;
    while (m % 2 == 0) {
        m /= 2;
        k++;
    }

    for (int i = 0; i < round_count; i++) {
        mpz_class a;
        mpz_rrandomb(a.get_mpz_t(), randstate, 2047);
        a %= n;
        while (a <= 1 or a == n - 1) {
            mpz_rrandomb(a.get_mpz_t(), randstate, 2047);
            a %= n;
        }
        mpz_class y;
        mpz_powm(y.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t(), n.get_mpz_t());
        if (y != n - 1 && y != 1) {
            for (mpz_class j = 0; j < k; j++) {
                mpz_class temp;
                mpz_powm(temp.get_mpz_t(), y.get_mpz_t(), MPZ_TWO.get_mpz_t(), n.get_mpz_t());
                y = temp;
                if (y == 1) return false;
                if (y == n - 1) break;
            }
            if (y != n - 1) return false;
        }
    }
    return true;
}
mpz_class generate_prime(int bits) {
    mpz_class to_ret;
    mpz_rrandomb(to_ret.get_mpz_t(), randstate, bits - 1);
    to_ret = to_ret * 2 + 1;
    while (!miller_rabin(to_ret)) to_ret += 2;
    return to_ret;
}

mpz_class encrypt(mpz_class& message, mpz_class& n, mpz_class& e) {
    mpz_class to_ret;
    mpz_powm(to_ret.get_mpz_t(), message.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
    return to_ret;
}

mpz_class decrypt(mpz_class& cyphertext, mpz_class& n, mpz_class& d) {
    mpz_class to_ret;
    mpz_powm(to_ret.get_mpz_t(), cyphertext.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
    return to_ret;
}

inline int convhex(char c) {return (c >= '0' and c <= '9') ? c - '0' : c - 'a' + 10;}


int main() {
    gmp_randinit_mt(randstate);
    gmp_randseed_ui(randstate, time(nullptr));

    char command;
    mpz_class n, d, e;

    while (true) {
        cout << "Type in d for decryption, e for encryption, g for key generation or others to quit: ";
        cin >> command;
        if (command == 'e') {
            char* n_str = new char[RSA_BITS];
            cout << "Type in the value of n (-1 to use previous value):";
            cin >> n_str;
            if (strcmp(n_str, "-1") != 0) mpz_set_str(n.get_mpz_t(), n_str, 16);
            cout << "Type in the value of e (-1 to use previous value):";
            cin >> n_str;
            if (strcmp(n_str, "-1") != 0) mpz_set_str(e.get_mpz_t(), n_str, 16);
            delete[] n_str;

            char* raw_message = new char[RSA_BITS / 8 - 1];
            cout << "Input message: ";
            // cin >> raw_message;
            fflush(stdin);
            size_t maxlen = RSA_BITS / 8 - 1;
            getline(&raw_message, &maxlen, stdin);
            mpz_class mpz_message;
            mpz_import(mpz_message.get_mpz_t(), strlen(raw_message), 1, sizeof(char), 0, 0, raw_message);
            delete[] raw_message;
            auto result = encrypt(mpz_message, n, e);
            cout << "Encrypted message: " << result.get_str(16);
        } else if (command == 'd') {
            char* n_str = new char[RSA_BITS];
            cout << "Type in the value of n (-1 to use previous value):";
            cin >> n_str;
            if (strcmp(n_str, "-1") != 0) mpz_set_str(n.get_mpz_t(), n_str, 16);
            cout << "Type in the value of d (-1 to use previous value):";
            cin >> n_str;
            if (strcmp(n_str, "-1") != 0) mpz_set_str(d.get_mpz_t(), n_str, 16);
            delete[] n_str;

            char* raw_message = new char[RSA_BITS];
            cout << "Input message: ";
            cin >> raw_message;
            mpz_class mpz_message;
            mpz_set_str(mpz_message.get_mpz_t(), raw_message, 16);
            delete[] raw_message;
            auto result = decrypt(mpz_message, n, d);

            cout << "Decrypted message: ";
            string buffer = result.get_str(16);
            auto length = buffer.length();
            if (length % 2) buffer = "0" + buffer;
            for (int j = 0; j < length / 2; j++) {
                int local = convhex(buffer[j * 2]) * 16 + convhex(buffer[j * 2 + 1]);
                if (local >= 128) local -= 255;
                cout << char(local);
            }
        } else if (command == 'g') {
            mpz_class p, q;
            p = generate_prime(RSA_BITS / 2);
            q = generate_prime(RSA_BITS / 2);
            n = p * q;
            cout << "Generated public key:\n\nn = " << n.get_str(16) << endl;
            mpz_class phi = (p - 1) * (q - 1);
            mpz_class gcd = 0;
            while (gcd != 1) {
                e = generate_prime(16);
                mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
            }
            cout << "e = " << e.get_str(16) << endl << endl;
            mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
            cout << "Generated private key:\n\nd = " << d.get_str(16) << endl << endl;
            // p and q are discarded
        } else break;
        cout << endl;
    }

    return 0;
}
