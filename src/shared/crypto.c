#include "crypto.h"

U16 fnv1b16(U8 *bytes, UNAT length)
{
	U32 prefold_hash = 0x811C9DC5;

	while (length--) {
		prefold_hash ^= *bytes++;
		prefold_hash *= 0x01000193;
	}

	return (prefold_hash & 0x0000FFFF) ^ ((prefold_hash & 0xFFFF0000) >> 16);
}