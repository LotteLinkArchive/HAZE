#include "crypto.h"

U16 fnv1b16(const U8 *bytes, SX length)
{
	U32 prefold_hash = 0x811C9DC5;

	while (length--) {
		prefold_hash ^= *bytes++;
		prefold_hash *= 0x01000193;
	}

	return (prefold_hash & 0x0000FFFF) ^ ((prefold_hash & 0xFFFF0000) >> 16);
}

U64 fnv1a64(const U8 *bytes, SX length)
{
	U64 hash = 0xCBF29CE484222325;

	while (length--) {
		hash ^= *bytes++;
		hash *= 0x00000100000001B3;
	}

	return hash;
}