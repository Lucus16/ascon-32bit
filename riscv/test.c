#include <stdio.h>

#include "ref/api.h"
#include "testcases/ascon128.h"

int main() {
    for (size_t i = 0; i < TESTCASE_COUNT; i++) {
        run_testcase(i);
    }
}

int run_testcase(size_t tc) {
    unsigned char encrypt_result[sizeof(ct)], decrypt_result[sizeof(pt)];
    unsigned long long encrypt_result_len, decrypt_result_len;
    int main_rv = 0;

    crypto_aead_encrypt(
            encrypt_result, &encrypt_result_len,
            pt, sizeof(pt),
            ad, sizeof(ad),
            NULL,
            pub_msg_nums[tc],
            keys[tc]);

    int rv = crypto_aead_decrypt(
            decrypt_result, &decrypt_result_len,
            NULL,
            encrypt_result, sizeof(encrypt_result),
            ad, sizeof(ad),
            pub_msg_nums[tc],
            keys[tc]);

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
