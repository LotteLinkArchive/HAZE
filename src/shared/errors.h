#ifndef HzERR_HEADER_FILE
#define HZERR_HEADER_FILE
#include "global.h"

enum glblstate {
	ST_NO_ERROR, /* no error */
	ST_MALLOC_FAILURE, /* out of memory */
	ST_MAP_FAILURE, /* mmap failure */
	ST_FILE_FAILURE, /* failed to read/write/open file */
	ST_CORRUPT_DATA, /* Integrity failure */
	ST_INVALID_DATA, /* Nonsense values, like a texture of size 10000000x10000000 */
	ST_BUFFER_OVERFLOW, /* attempted buffer overflow */
	ST_UNSUPPORTED /* Tried to do something that isn't supported */
};

typedef U8 STATUS;

#endif
