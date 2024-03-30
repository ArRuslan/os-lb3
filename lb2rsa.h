#pragma once
#include <cstdint>

struct PQ {
    uint32_t p;
    uint32_t q;
};

struct PubKey {
    uint32_t e;
    uint64_t n;
};

struct PrivKey {
    uint64_t d;
    uint64_t n;
};

class RSA {
public:
    RSA();
    uint64_t encrypt(uint64_t data);
    uint64_t decrypt(uint64_t data);

private:
    PubKey pub_key{};
    PrivKey priv_key{};
};
