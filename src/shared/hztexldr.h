#ifndef HZTEXLDR_HEADER_FILE
#define HZTEXLDR_HEADER_FILE
#include "srtex.h"
#include "errors.h"

/* Struct for a loaded texture */
struct hztex_loaded {
	U16 width;
	U16 height;
	SX data_length;
	X0 *pixels; /* Must be a RGBA 32bpp (U8 red, U8 green, U8 blue, U8 alpha) surface of data_length bytes */
	X0 *free_address; /* Address to free/munmap when done - only set if malloc/mmap is handled by hztexldr. */
	U1 mmapped; /* Use munmap */
	SX munmap_size; /* munmap data size */
};

/* Interprets a SRTEX/.srt, located at <blob> of length <size_boundary>.
 * If status is okay, will populate all values in the hztex_loaded output struct.
 */
STATUS hztex_torgba32(X0 *blob, SX size_boundary, struct hztex_loaded *output);

/* Efficient loading of textures from files via file descriptor, size and offset. Will malloc or mmap. */
STATUS hztex_fd_torgba32(INAT fd, SX size, OX offset, struct hztex_loaded *output);

/* Free a surface/texture after it has been loaded, in case you used a function like hztex_fd_torgba32 */
STATUS hztex_free_surf(struct hztex_loaded *output);
#endif