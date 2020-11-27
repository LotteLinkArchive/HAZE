#include "errors.h"

const CHR *hz_err_string(STATUS code) {
	switch (code) {
	case ST_NO_ERROR:
		return "ST_NO_ERROR: No error triggered. This isn't supposed to happen.";
	case ST_MALLOC_FAILURE:
		return "ST_MALLOC_FAILURE: Out of memory or otherwise incapable of allocating new memory regions.";
	case ST_MAP_FAILURE:
		return "ST_MAP_FAILURE: Unable to manipulate the virtual address space via memory mapping.";
	case ST_FILE_FAILURE:
		return "ST_FILE_FAILURE: Failed to read/write/open an important file.";
	case ST_CORRUPT_DATA:
		return "ST_CORRUPT_DATA: Critical integrity failure. Ensure provided data is not corrupt in some way.";
	case ST_INVALID_DATA:
		return "ST_INVALID_DATA: Nonsense data provided. Unable to safely continue.";
	case ST_BUFFER_OVERFLOW:
		return "ST_BUFFER_OVERFLOW: Detected and prevented a potentially invalid memory access.";
	case ST_UNSUPPORTED:
		return "ST_UNSUPPORTED: Attempted to use an unsupported feature.";
	case ST_NULL_FED:
		return "ST_NULL_FED: Invalid pointer provided, was not expecting NULL.";
	}

	return "Unknown error. Provided status code was invalid.";
}