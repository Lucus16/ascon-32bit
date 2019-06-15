#include <stdlib.h>
#include <stdio.h>

#include "api.h"
#include "crypto_aead.h"
#include "tests.h"

#define REPETITIONS 0x20
#define ROUNDS 120

extern unsigned int getcycles();
extern void permutation(unsigned char *state, int start);

int compare_ints(const void *a, const void *b) {
    return *(const int *)b - *(const int *)a;
}

void permutation_benchmark() {
    unsigned char buf[40];
    unsigned int permutation_times[REPETITIONS];
    unsigned int empty_times[REPETITIONS];

    for (size_t i = 0; i < REPETITIONS; i++) {
        permutation_times[i] = -getcycles();
        permutation(buf, 2 * ROUNDS);
        permutation_times[i] += getcycles();
    }

    for (size_t i = 0; i < REPETITIONS; i++) {
        empty_times[i] = -getcycles();
        permutation(buf, ROUNDS);
        empty_times[i] += getcycles();
    }

    qsort(permutation_times, REPETITIONS, sizeof(unsigned int), compare_ints);
    qsort(empty_times, REPETITIONS, sizeof(unsigned int), compare_ints);

    printf("cycles for %d permutation rounds: %d\n", ROUNDS,
            permutation_times[REPETITIONS / 2] - empty_times[REPETITIONS / 2]);
}

int run_testcase(size_t tc_index) {
    struct testcase tc = testcases[tc_index];
    unsigned char encrypt_result[tc.ct_size + TAG_SIZE];
    unsigned char decrypt_result[tc.pt_size];
    unsigned long long encrypt_result_size, decrypt_result_size;
    unsigned int encrypt_times[REPETITIONS], decrypt_times[REPETITIONS];
    int decrypt_rv, return_value = 0;

    for (size_t i = 0; i < REPETITIONS; i++) {
        encrypt_times[i] = -getcycles();
        crypto_aead_encrypt(
                encrypt_result, &encrypt_result_size,
                tc.pt, tc.pt_size,
                tc.ad, tc.ad_size,
                NULL, tc.npub, tc.key);
        encrypt_times[i] += getcycles();
    }

    for (size_t i = 0; i < REPETITIONS; i++) {
        decrypt_times[i] = -getcycles();
        decrypt_rv = crypto_aead_decrypt(
                decrypt_result, &decrypt_result_size,
                NULL,
                encrypt_result, tc.ct_size + TAG_SIZE,
                tc.ad, tc.ad_size,
                tc.npub, tc.key);
        decrypt_times[i] += getcycles();
    }

    qsort(encrypt_times, REPETITIONS, sizeof(unsigned int), compare_ints);
    qsort(decrypt_times, REPETITIONS, sizeof(unsigned int), compare_ints);

    for (size_t i = 0; i < tc.ct_size; i++) {
        if (encrypt_result[i] != tc.ct[i]) {
            printf("%2d encrypt: At %d, expected %02x, got %02x.\n",
                    tc_index, i, tc.ct[i], encrypt_result[i]);
            return_value = 1;
            break;
        }
    }

    for (size_t i = 0; i < TAG_SIZE; i++) {
        if (encrypt_result[tc.ct_size + i] != tc.tag[i]) {
            printf("%2d authtag: At %d, expected %02x, got %02x.\n",
                    tc_index, i, tc.tag[i], encrypt_result[tc.ct_size + i]);
            return_value = 1;
            break;
        }
    }

    if (decrypt_rv != 0) {
        printf("%2d decrypt: Error code is %d.\n", tc_index, decrypt_rv);
        return_value = 1;
    }

    for (size_t i = 0; i < tc.pt_size; i++) {
        if (decrypt_result[i] != tc.pt[i]) {
            printf("%2d decrypt: At %d, expected %02x, got %02x.\n",
                    tc_index, i, tc.pt[i], decrypt_result[i]);
            return_value = 1;
            break;
        }
    }

    if (return_value == 0) {
        printf("%2d Test passed! %4lld bytes, encrypt in %6d, "
                "decrypt in %6d cycles.\n",
                tc_index, tc.pt_size + tc.ad_size,
                encrypt_times[REPETITIONS / 2],
                decrypt_times[REPETITIONS / 2]);
    }

    return return_value;
}

int main() {
    printf("Ready.\n");
    int return_value = 0;

    for (size_t i = 0; i < TESTCASE_COUNT; i++) {
        return_value |= run_testcase(i);
    }

    permutation_benchmark();

    return return_value;
}
