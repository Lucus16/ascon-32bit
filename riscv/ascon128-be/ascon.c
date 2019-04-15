#include <stdio.h>
#include "api.h"
#include "crypto_aead.h"

#define KLEN CRYPTO_KEYBYTES
#define SIZE (320 / 8)
#define RATE (64 / 8)
#define CAPACITY (SIZE - RATE)
#define PARAM_A 12
#define PARAM_B 6
#define ROUNDS_END 12

#define REVERSED_STATE(i) state[SIZE - 1 - (i)]
#define ROTR(x,n) (((x)>>(n))|((x)<<(64-(n))))

typedef unsigned char u8;
typedef unsigned long long u64;
typedef long long i64;

//#define PRINTSTATE
//#define PRINTWORDS

void printstate(char* text, u8* state) {
#ifdef PRINTSTATE
  int i;
  printf("%s\n", text);
  for (i = 0; i < 40; ++i)
    printf("%02x", REVERSED_STATE(i));
  printf("\n");
#endif
}

void printwords(char* text, u64 x0, u64 x1, u64 x2, u64 x3, u64 x4) {
#ifdef PRINTWORDS
  int i;
  printf("%s\n", text);
  printf("  x0=%016llx\n", x0);
  printf("  x1=%016llx\n", x1);
  printf("  x2=%016llx\n", x2);
  printf("  x3=%016llx\n", x3);
  printf("  x4=%016llx\n", x4);
#endif
}

void permutation(u8 *state_bytes, int rounds) {
  u64 *state = (u64*)state_bytes;
  int i;
  u64 x0, x1, x2, x3, x4;
  u64 t0, t1, t2, t3, t4;
  x0 = state[4];
  x1 = state[3];
  x2 = state[2];
  x3 = state[1];
  x4 = state[0];
  printwords(" permutation input:", x0, x1, x2, x3, x4);
  for (i = ROUNDS_END - rounds; i < ROUNDS_END; ++i) {
    // addition of round constant
    x2 ^= 0xf0 - i * 0x0f;
    printwords(" addition of round constant:", x0, x1, x2, x3, x4);
    // substitution layer
    x0 ^= x4;    x4 ^= x3;    x2 ^= x1;
    t0  = x0;    t1  = x1;    t2  = x2;    t3  = x3;    t4  = x4;
    t0 =~ t0;    t1 =~ t1;    t2 =~ t2;    t3 =~ t3;    t4 =~ t4;
    t0 &= x1;    t1 &= x2;    t2 &= x3;    t3 &= x4;    t4 &= x0;
    x0 ^= t1;    x1 ^= t2;    x2 ^= t3;    x3 ^= t4;    x4 ^= t0;
    x1 ^= x0;    x0 ^= x4;    x3 ^= x2;    x2 =~ x2;
    printwords(" substitution layer:", x0, x1, x2, x3, x4);
    // linear diffusion layer
    x0 ^= ROTR(x0, 19) ^ ROTR(x0, 28);
    x1 ^= ROTR(x1, 61) ^ ROTR(x1, 39);
    x2 ^= ROTR(x2,  1) ^ ROTR(x2,  6);
    x3 ^= ROTR(x3, 10) ^ ROTR(x3, 17);
    x4 ^= ROTR(x4,  7) ^ ROTR(x4, 41);
    printwords(" linear diffusion layer:", x0, x1, x2, x3, x4);
  }
  state[0] = x4;
  state[1] = x3;
  state[2] = x2;
  state[3] = x1;
  state[4] = x0;
}

int crypto_aead_encrypt(
    unsigned char *c, unsigned long long *clen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k) {

  u64 s = adlen / RATE + 1;
  u64 t = mlen / RATE + 1;
  u64 l = mlen % RATE;

  u8 state[SIZE];
  u8 A[s * RATE];
  u8 M[t * RATE];
  u64 i, j;

  // pad associated data
  for (i = 0; i < adlen; ++i)
    A[i] = ad[i];
  A[adlen] = 0x80;
  for (i = adlen + 1; i < s * RATE; ++i)
    A[i] = 0;
  // pad plaintext
  for (i = 0; i < mlen; ++i)
    M[i] = m[i];
  M[mlen] = 0x80;
  for (i = mlen + 1; i < t * RATE; ++i)
    M[i] = 0;

  // initialization
  state[39] = KLEN * 8;
  state[38] = RATE * 8;
  state[37] = PARAM_A;
  state[36] = PARAM_B;
  for (i = 4; i < SIZE - 2 * KLEN; ++i)
    REVERSED_STATE(i) = 0;
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(SIZE - 2 * KLEN + i) = k[i];
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(SIZE - KLEN + i) = npub[i];
  printstate("initial value:", state);
  permutation(state, PARAM_A);
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + KLEN + i) ^= k[i];
  printstate("initialization:", state);

  // process associated data
  if (adlen != 0) {
    for (i = 0; i < s; ++i) {
      for (j = 0; j < RATE; ++j)
        REVERSED_STATE(j) ^= A[i * RATE + j];
      permutation(state, PARAM_B);
    }
  }
  REVERSED_STATE(SIZE - 1) ^= 1;
  printstate("process associated data:", state);

  // process plaintext
  for (i = 0; i < t - 1; ++i) {
    for (j = 0; j < RATE; ++j) {
      REVERSED_STATE(j) ^= M[i * RATE + j];
      c[i * RATE + j] = REVERSED_STATE(j);
    }
    permutation(state, PARAM_B);
  }
  for (j = 0; j < RATE; ++j)
    REVERSED_STATE(j) ^= M[(t - 1) * RATE + j];
  for (j = 0; j < l; ++j)
    c[(t - 1) * RATE + j] = REVERSED_STATE(j);
  printstate("process plaintext:", state);

  // finalization
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + i) ^= k[i];
  permutation(state, PARAM_A);
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + KLEN + i) ^= k[i];
  printstate("finalization:", state);

  // return tag
  for (i = 0; i < KLEN; ++i)
    c[mlen + i] = REVERSED_STATE(RATE + KLEN + i);
  *clen = mlen + KLEN;

  return 0;
}

int crypto_aead_decrypt(
    unsigned char *m, unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c, unsigned long long clen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k) {

  *mlen = 0;
  if (clen < CRYPTO_KEYBYTES)
    return -1;

  u64 s = adlen / RATE + 1;
  u64 t = (clen - KLEN) / RATE + 1;
  u64 l = (clen - KLEN) % RATE;

  u8 state[SIZE];
  u8 A[s * RATE];
  u8 M[t * RATE];
  u64 i, j;

  // pad associated data
  for (i = 0; i < adlen; ++i)
    A[i] = ad[i];
  A[adlen] = 0x80;
  for (i = adlen + 1; i < s * RATE; ++i)
    A[i] = 0;

  // initialization
  REVERSED_STATE(0) = KLEN * 8;
  REVERSED_STATE(1) = RATE * 8;
  REVERSED_STATE(2) = PARAM_A;
  REVERSED_STATE(3) = PARAM_B;
  for (i = 4; i < SIZE - 2 * KLEN; ++i)
    REVERSED_STATE(i) = 0;
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(SIZE - 2 * KLEN + i) = k[i];
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(SIZE - KLEN + i) = npub[i];
  printstate("initial value:", state);
  permutation(state, PARAM_A);
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + KLEN + i) ^= k[i];
  printstate("initialization:", state);

  // process associated data
  if (adlen) {
    for (i = 0; i < s; ++i) {
      for (j = 0; j < RATE; ++j)
        REVERSED_STATE(j) ^= A[i * RATE + j];
      permutation(state, PARAM_B);
    }
  }
  REVERSED_STATE(SIZE - 1) ^= 1;
  printstate("process associated data:", state);

  // process plaintext
  for (i = 0; i < t - 1; ++i) {
    for (j = 0; j < RATE; ++j) {
      M[i * RATE + j] = REVERSED_STATE(j) ^ c[i * RATE + j];
      REVERSED_STATE(j) = c[i * RATE + j];
    }
    permutation(state, PARAM_B);
  }
  for (j = 0; j < l; ++j)
    M[(t - 1) * RATE + j] = REVERSED_STATE(j) ^ c[(t - 1) * RATE + j];
  for (j = 0; j < l; ++j)
    REVERSED_STATE(j) = c[(t - 1) * RATE + j];
  REVERSED_STATE(l) ^= 0x80;
  printstate("process plaintext:", state);

  // finalization
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + i) ^= k[i];
  permutation(state, PARAM_A);
  for (i = 0; i < KLEN; ++i)
    REVERSED_STATE(RATE + KLEN + i) ^= k[i];
  printstate("finalization:", state);

  // return -1 if verification fails
  for (i = 0; i < KLEN; ++i)
    if (c[clen - KLEN + i] != REVERSED_STATE(RATE + KLEN + i))
      return -1;

  // return plaintext
  *mlen = clen - KLEN;
  for (i = 0; i < *mlen; ++i)
    m[i] = M[i];

  return 0;
}

#undef SIZE
#undef RATE
#undef CAPACITY
#undef PARAM_A
#undef PARAM_B
#undef REVERSED_STATE
