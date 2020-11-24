#ifndef HZTEXLDR_HEADER_FILE
#define HZTEXLDR_HEADER_FILE
#include "srtex.h"
#include "errors.h"

/* Interprets a SRTEX/.srt, located at <blob> of length <size_boundary>.
 * width, height, data_length and output are all pointers to outputs of this function and will be overwritten if they
 * are not NULL.
 *
 * width becomes the width, in pixels, of the image.
 * height becomes the height, in pixels, of the image.
 * data_length becomes the length, in bytes, of the image.
 * output becomes a pointer to a RGBA 32bpp (U8 red, U8 green, U8 blue, U8 alpha) image.
 *
 * None of the values are set if STATUS is non-ok.
 */
STATUS hztex_torgba32(
	X0 *blob,
	SX size_boundary,
	U16 *width,
	U16 *height,
	SX *data_length,
	X0 **output);
#endif