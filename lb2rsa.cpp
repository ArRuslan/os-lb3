#include "lb2rsa.h"

#include <cmath>
#include <random>

#ifdef __MINGW32__
#include <windows.h>
#endif

uint32_t randrange(uint32_t min, uint32_t max) {
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<uint32_t> distr;

    uint32_t range = max - min + 1;
    return distr(eng) % range + min;
}

uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    __int128 x = 1, y = base;
    while (exp > 0) {
        if (exp % 2 == 1) {
            x = x * y % mod;
        }
        y = y * y % mod;
        exp /= 2;
    }
    return x % mod;
}

bool areCoprime(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a == 1;
}

uint32_t find_coprime(uint64_t m) {
    long long coprime = 2;
    while (!areCoprime(m, coprime)) {
        coprime++;
    }
    return coprime;
}

uint64_t find_mod_inverse(int64_t a, int64_t m) {
    int64_t oldR = a;
    int64_t r = m;
    int64_t oldS = 1;
    int64_t s = 0;

    while (r > 0) {
        int64_t quot = oldR / r;

        int64_t tempR = r;
        r = oldR - quot * r;
        oldR = tempR;

        int64_t tempS = s;
        s = oldS - quot * s;
        oldS = tempS;
    }

    if (oldS < 0)
        oldS += m;
    return oldS;
}

bool isPrime(uint32_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

uint32_t gen_prime() {
    while (true) {
        uint32_t to_check = randrange(1 << 30, 1 << 31);
        if (isPrime(to_check)) return to_check;
    }
}

PQ gen_pq() {
    uint32_t p = gen_prime(), q = gen_prime();
    while (p == q) {
        p = gen_prime();
        q = gen_prime();
    }
    return PQ{p, q};
}

RSA::RSA() {
    PQ pq = gen_pq();
    uint64_t phi_N = (uint64_t) (pq.p - 1) * (pq.q - 1);
    pub_key.e = find_coprime(phi_N);
    priv_key.d = find_mod_inverse(pub_key.e, phi_N);
    pub_key.n = priv_key.n = (uint64_t) pq.p * pq.q;
}

uint64_t RSA::encrypt(uint64_t data) {
    return modpow(data, pub_key.e, pub_key.n);
}

uint64_t RSA::decrypt(uint64_t data) {
    return modpow(data, priv_key.d, priv_key.n);
}

extern "C" {
RSA *new_RSA() {
    return new RSA();
}

uint64_t RSA_encrypt(RSA *rsa, uint64_t data) {
    return rsa->encrypt(data);
}

uint64_t RSA_decrypt(RSA *rsa, uint64_t data) {
    return rsa->decrypt(data);
}
}

#ifdef __MINGW32__
bool APIENTRY DllMain(HMODULE module, uint32_t call_reason, void *rsv) {
    switch (call_reason) {
        case DLL_PROCESS_ATTACH:
            printf("DLL_PROCESS_ATTACH\n");
            break;
        case DLL_THREAD_ATTACH:
            printf("DLL_THREAD_ATTACH\n");
            break;
        case DLL_THREAD_DETACH:
            printf("DLL_THREAD_DETACH\n");
            break;
        case DLL_PROCESS_DETACH:
            printf("DLL_PROCESS_DETACH\n");
            break;
    }
    return true;
}
#endif
