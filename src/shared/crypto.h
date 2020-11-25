/* HAZE Cryptography and/or Hashing functions */

#ifndef HZCRYPT_HEADER_FILE
#define HZCRYPT_HEADER_FILE
#include "global.h"

/* Calculate a 16-bit XOR-folded FNV1a checksum (becomes FNV1b) */
U16 fnv1b16(U8 *bytes, UNAT length);
#endif
