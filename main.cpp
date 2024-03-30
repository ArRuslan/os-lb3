#ifdef __MINGW32__
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

#include <cmath>
#include <iostream>
#include <random>
#include "lb2rsa.h"
#include "locales.h"

typedef RSA *(*new_RSA_t)();
typedef uint64_t(*RSA_encrypt_t)(RSA*, uint64_t);
typedef uint64_t(*RSA_decrypt_t)(RSA*, uint64_t);

void task1(new_RSA_t new_rsa, RSA_encrypt_t encrypt, RSA_decrypt_t decrypt) {
    RSA *d0e0n0 = new_rsa();
    RSA *d1e1n1 = new_rsa();
    uint64_t ti = 123456789;

    uint64_t e1t = encrypt(d1e1n1, ti);
    uint64_t d1e1t = decrypt(d1e1n1, e1t);
    if (d1e1t != ti) {
        printf("Original value and decrypted do not match!\n");
        return;
    }

    printf("Input: %ld\n", ti);
    printf("Encrypted: %ld\n", e1t);
    printf("Decrypted: %ld\n", d1e1t);
    printf("--------------------------\n");

    uint64_t e0d1e1t = encrypt(d0e0n0, d1e1t);
    uint64_t d0e0d1e1t = decrypt(d0e0n0, e0d1e1t);
    if (d0e0d1e1t != d1e1t) {
        printf("Original value and decrypted do not match!\n");
        return;
    }

    printf("Input: %ld\n", d1e1t);
    printf("Encrypted: %ld\n", e0d1e1t);
    printf("Decrypted: %ld\n", d0e0d1e1t);
    printf("--------------------------\n");

    delete d0e0n0;
    delete d1e1n1;
}

void task1() {
#ifdef __MINGW32__
    HMODULE handle = LoadLibrary("./liblb2rsa.so");
    if(!handle)
        return;

    auto new_rsa = reinterpret_cast<new_RSA_t>(GetProcAddress(handle, "new_RSA"));
    auto encrypt = reinterpret_cast<RSA_encrypt_t>(GetProcAddress(handle, "RSA_encrypt"));
    auto decrypt = reinterpret_cast<RSA_decrypt_t>(GetProcAddress(handle, "RSA_decrypt"));
#else
    void *lib_dyn = dlopen("./liblib_dyn.so", RTLD_LAZY);
    auto new_rsa = reinterpret_cast<new_RSA_t>(dlsym(lib_dyn, "new_RSA"));
    auto encrypt = reinterpret_cast<RSA_encrypt_t>(dlsym(lib_dyn, "RSA_encrypt"));
    auto decrypt = reinterpret_cast<RSA_decrypt_t>(dlsym(lib_dyn, "RSA_decrypt"));
#endif

    printf("Addresses:\n  new_RSA=%p\n  RSA_encrypt=%p\n  RSA_decrypt=%p\n\n", &new_rsa, &encrypt, &decrypt);
    task1(new_rsa, encrypt, decrypt);

#ifdef __MINGW32__
    FreeLibrary(handle);
#else
    dlclose(lib_dyn);
#endif
}

void task2() {
    setlocale(LC_ALL, "uk_UA.utf8");

    std::string locale;
    while(locale != "uk" && locale != "en") {
        printf("Enter locale (en/uk): ");
        std::getline(std::cin, locale);
    }

    HMODULE handle = LoadLibrary(("./liblocale_"+locale+".so").c_str());

    char* res_buf = new char[64];
    LoadString(handle, RES_NAME, res_buf, 63);
    printf("  RES_NAME = %s\n", res_buf);
    LoadString(handle, RES_FACULTY, res_buf, 63);
    printf("  RES_FACULTY = %s\n", res_buf);
    LoadString(handle, RES_GROUP, res_buf, 63);
    printf("  RES_GROUP = %s\n", res_buf);
    LoadString(handle, RES_DISC, res_buf, 63);
    printf("  RES_DISC = %s\n", res_buf);

    delete[] res_buf;
    FreeLibrary(handle);
}

int main() {
    task1();
    task2();

    return 0;
}
