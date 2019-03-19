#include <stdint.h>
#include <stdio.h>

#include "ref/api.h"

const uint8_t key[] = {
    0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C,
    0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64,
};

const uint8_t npub[] = {
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
};

const uint8_t ad[] = {
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
};

const uint8_t pt[] = {
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
};

const uint8_t ct[] = {
    0xAA, 0xA3, 0xB3, 0x68, 0x29, 0xA2, 0xF0, 0x26,
    0x8F, 0xED, 0xD9, 0x53, 0x0A, 0x97, 0x65, 0xA9,
    0x3F, 0x42, 0x32, 0xF3, 0x7B, 0x1F, 0xF5, 0xFE,
    0x2B, 0x6C, 0xAE, 0xB8, 0x79, 0x25, 0x54, 0x7C,
};

int main() {
    uint8_t encrypt_result[sizeof(ct)], decrypt_result[sizeof(pt)];
    uint64_t encrypt_result_len, decrypt_result_len;
    int main_rv = 0;

    crypto_aead_encrypt(
            encrypt_result, &encrypt_result_len,
            pt, sizeof(pt),
            ad, sizeof(ad),
            NULL,
            npub,
            key);

    int rv = crypto_aead_decrypt(
            decrypt_result, &decrypt_result_len,
            NULL,
            encrypt_result, sizeof(encrypt_result),
            ad, sizeof(ad),
            npub,
            key);

    for (size_t i = 0; i < sizeof(ct); i++) {
        if (encrypt_result[i] != ct[i]) {
            printf("encrypt: At %d, expected %02x, got %02x.\n", i, ct[i],
                    encrypt_result[i]);
            main_rv = 1;
            break;
        }
    }

    if (rv != 0) {
        printf("decrypt: Error code is %d.\n", rv);
        main_rv = 2;
    } else {
        for (size_t i = 0; i < sizeof(pt); i++) {
            if (decrypt_result[i] != pt[i]) {
                printf("decrypt: At %d, expected %02x, got %02x.\n", i, pt[i],
                        decrypt_result[i]);
                main_rv = 3;
                break;
            }
        }
    }

    if (main_rv == 0) {
        printf("All tests passed!\n");
    }

    return 0;
}
