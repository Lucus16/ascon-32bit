#include <stdio.h>

#include "api.h"
#include "crypto_aead.h"
#include "tests.h"

extern unsigned int getcycles();

int run_testcase(size_t tc) {
    unsigned char encrypt_result[cipher_text_sizes[tc] + CRYPTO_ABYTES];
    unsigned char decrypt_result[plain_text_sizes[tc]];
    unsigned long long encrypt_result_len, decrypt_result_len;
    int return_value = 0, decrypt_rv;
    unsigned int encrypt_cycles, decrypt_cycles;

    // Get the instructions cached.
    for (size_t i = 0; i < 0x10; i++)
        crypto_aead_encrypt(
                encrypt_result, &encrypt_result_len,
                plain_texts[tc], plain_text_sizes[tc],
                additionals[tc], additional_sizes[tc],
                NULL,
                pub_msg_nums[tc],
                keys[tc]);

    encrypt_cycles = -getcycles();
    crypto_aead_encrypt(
            encrypt_result, &encrypt_result_len,
            plain_texts[tc], plain_text_sizes[tc],
            additionals[tc], additional_sizes[tc],
            NULL,
            pub_msg_nums[tc],
            keys[tc]);
    encrypt_cycles += getcycles();

    // Get the instructions cached.
    for (size_t i = 0; i < 0x10; i++)
        decrypt_rv = crypto_aead_decrypt(
                decrypt_result, &decrypt_result_len,
                NULL,
                encrypt_result, cipher_text_sizes[tc] + CRYPTO_ABYTES,
                additionals[tc], additional_sizes[tc],
                pub_msg_nums[tc],
                keys[tc]);

    decrypt_cycles = -getcycles();
    decrypt_rv = crypto_aead_decrypt(
            decrypt_result, &decrypt_result_len,
            NULL,
            encrypt_result, cipher_text_sizes[tc] + CRYPTO_ABYTES,
            additionals[tc], additional_sizes[tc],
            pub_msg_nums[tc],
            keys[tc]);
    decrypt_cycles += getcycles();

    for (size_t i = 0; i < cipher_text_sizes[tc]; i++) {
        if (encrypt_result[i] != cipher_texts[tc][i]) {
            printf("%2d encrypt: At %d, expected %02x, got %02x.\n", tc, i,
                    cipher_texts[tc][i], encrypt_result[i]);
            return_value = 1;
            break;
        }
    }

    for (size_t i = 0; i < CRYPTO_ABYTES; i++) {
        if (encrypt_result[cipher_text_sizes[tc] + i] != auth_tags[tc][i]) {
            printf("%2d authtag: At %d, expected %02x, got %02x.\n", tc, i,
                    auth_tags[tc][i], encrypt_result[cipher_text_sizes[tc] + i]);
            return_value = 1;
            break;
        }
    }

    if (decrypt_rv != 0) {
        printf("%2d decrypt: Error code is %d.\n", tc, decrypt_rv);
        return_value = 1;
    }

    for (size_t i = 0; i < plain_text_sizes[tc]; i++) {
        if (decrypt_result[i] != plain_texts[tc][i]) {
            printf("%2d decrypt: At %d, expected %02x, got %02x.\n", tc, i,
                    plain_texts[tc][i], decrypt_result[i]);
            return_value = 1;
            break;
        }
    }

    if (return_value == 0) {
        printf("%2d Test passed! %4lld bytes, encrypt in %6d, "
                "decrypt in %6d cycles.\n",
                tc, plain_text_sizes[tc] + additional_sizes[tc],
                encrypt_cycles, decrypt_cycles);
    }

    return 0;
}

int main() {
    printf("Ready.\n");
    int return_value = 0;

    for (size_t i = 0; i < TESTCASE_COUNT; i++) {
        return_value |= run_testcase(i);
    }

    return return_value;
}
