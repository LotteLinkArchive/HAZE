#ifndef SURTXD_HEADER_FILE
#define SURTXD_HEADER_FILE
#include "global.h"

#define SRT_MAGIC_NUMBER 0xEAE5833F
#define SRT_INIT { .magicno = SRT_MAGIC_NUMBER }

/* Defines the structure of an SRT texture header. */
__extension__ struct srtexhdr {
	/* The SRT magic number, 0xEAE5833F */
	U32 magicno;

	/* The width and height of the SRT texture */
	U32 width;
	U32 height;

	/* The data length in bytes */
	U32 data_length;

	/* Bytes per pixel */
	U8  Bpp;

	/* Amount of bytes between the end of the header and the start of the image body */
	U8  offset;

	/* 16-bit FNV-1b (defined above) hash of the image body */
	U16 checksum;

	/* After the header is written to the file, the entire rest of the file should be of size data_length and
	 * contain the required image data.
	 */
} __attribute__ ((packed));

#define SRT_HEADER_WIDTH sizeof(struct srtexhdr)

#endif
