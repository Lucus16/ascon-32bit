#define CRYPTO_KEYBYTES 16
#define CRYPTO_NSECBYTES 0
#define CRYPTO_NPUBBYTES 16
#define CRYPTO_ABYTES 16
#define CRYPTO_NOOVERLAP 1

typedef unsigned char u8;
typedef unsigned long long u64;
typedef long long i64;

void printstate(char* text, u8* S);
void printwords(char* text, u64 x0, u64 x1, u64 x2, u64 x3, u64 x4);
void load64(u64* x, u8* S);
void store64(u8* S, u64 x);
void permutation(u8* S, int start, int rounds);

int crypto_aead_encrypt(
    u8 *c, u64 *clen,
    const u8 *m, u64 mlen,
    const u8 *ad, u64 adlen,
    const u8 *nsec,
    const u8 *npub,
    const u8 *k);

int crypto_aead_decrypt(
    u8 *m, u64 *mlen,
    u8 *nsec,
    const u8 *c, u64 clen,
    const u8 *ad, u64 adlen,
    const u8 *npub,
    const u8 *k);
