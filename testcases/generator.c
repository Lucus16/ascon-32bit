#include <sys/random.h>
#include <stdio.h>

#include "api.h"
#include "crypto_aead.h"

#ifndef PT_SIZE
#define PT_SIZE 4096
#endif

int main() {
    unsigned int i;
    unsigned long long ct_size;
    unsigned char key[CRYPTO_KEYBYTES];
    unsigned char npub[CRYPTO_NPUBBYTES];
    unsigned char pt[PT_SIZE];
    unsigned char ct[PT_SIZE + CRYPTO_ABYTES];
    unsigned char *tag = ct + PT_SIZE;

    while (getrandom(pt, PT_SIZE, 0) != PT_SIZE);
    while (getrandom(key, CRYPTO_KEYBYTES, 0) != CRYPTO_KEYBYTES);
    while (getrandom(npub, CRYPTO_NPUBBYTES, 0) != CRYPTO_NPUBBYTES);
    crypto_aead_encrypt(ct, &ct_size, pt, PT_SIZE, 0, 0, 0, npub, key);

    printf("key     = ");
    for (i = 0; i < CRYPTO_KEYBYTES; i++) {
        printf("%02X", key[i]);
    }

    printf("\nnpub    = ");
    for (i = 0; i < CRYPTO_NPUBBYTES; i++) {
        printf("%02X", npub[i]);
    }

    printf("\nad      =");

    printf("\npt      = ");
    for (i = 0; i < PT_SIZE; i++) {
        printf("%02X", pt[i]);
    }

    printf("\nct      = ");
    for (i = 0; i < PT_SIZE; i++) {
        printf("%02X", ct[i]);
    }

    printf("\ntag     = ");
    for (i = 0; i < CRYPTO_ABYTES; i++) {
        printf("%02X", tag[i]);
    }

    printf("\n\n");
}
