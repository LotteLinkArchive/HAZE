/* HAZE Cryptography and/or Hashing functions */

#ifndef HZCRYPT_HEADER_FILE
#define HZCRYPT_HEADER_FILE
#include "global.h"
#include <xxhash.h>

/* Calculate a 16-bit XOR-folded FNV1a checksum (becomes FNV1b) */
H_EXTERN U16 fnv1b16(const U8 *bytes, SX length);
/* Calculate a 64-bit FNV1a checksum */
H_EXTERN U64 fnv1a64(const U8 *bytes, SX length);

#define xxh3_64b(bytes, length) XXH3_64bits(bytes, length)
#define STRHASH(STR) xxh3_64b((U8 *)(STR), strlen(STR) * sizeof(CHR))
#endif
