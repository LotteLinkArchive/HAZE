#include "hztexldr.h"

STATUS hztex_torgba32(
	X0 *blob,
	SX size_boundary,
	U16 *width,
	U16 *height,
	SX *data_length,
	X0 **output)
{
	/* Prepare to read the header */
	struct SRTHeader *head = blob;

	/* Perform careful boundary checking before accessing anything to try to prevent a buffer overflow */
	if  (size_boundary  < SRT_HEADER_WIDTH)                            return ST_BUFFER_OVERFLOW;
	if  (head->magicno != SRT_MAGIC_NUMBER)                            return ST_INVALID_DATA;
	if ((head->height * head->width * head->Bpp) != head->data_length) return ST_INVALID_DATA;

	/* Header is clean by now */
	SX dangerzone = SRT_HEADER_WIDTH + head->offset;

	/* Check canvas eligbility and data body bounds */
	if  (head->data_length > (size_boundary - dangerzone))             return ST_BUFFER_OVERFLOW;
	if  (head->Bpp  != 4)                                              return ST_UNSUPPORTED;
	if ((head->width < 1) || (head->height < 1))                       return ST_INVALID_DATA;

	/* Define the boundary between header and image content */
	X0 *body = (U8 *)blob + dangerzone;

	/* Verify the image body checksum */
	if (head->checksum != fnv1b16((U8 *)body, head->data_length))      return ST_CORRUPT_DATA;

	if (output)      *output      = body;
	if (width)       *width       = head->width;
	if (height)      *height      = head->height;
	if (data_length) *data_length = head->data_length; 

	/* Return the OK state */
	return ST_NO_ERROR;
}
