#include "hztexldr.h"
#include "umman.h"

STATUS hztex_torgba32(X0 *blob, SX size_boundary, struct hztex_loaded *output)
{
	/* Prepare to read the header */
	struct srtexhdr *head = blob;

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

	output->pixels      = body;
	output->width       = head->width;
	output->height      = head->height;
	output->data_length = head->data_length; 

	/* Return the OK state */
	return ST_NO_ERROR;
}

STATUS hztex_fd_torgba32(INAT fd, SX size, OX offset, struct hztex_loaded *output)
{
	/* Invalidate invalid sizes */
	if (size < SRT_HEADER_WIDTH) return ST_INVALID_DATA;

	/* Map the file to memory */
	X0 *mapped = ummap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);

	/* If the map failed, return now. */
	if (!mapped) return ST_MAP_FAILURE;

	/* Try to create a canvas out of it */
	STATUS decstat = hztex_torgba32(mapped, size, output);

	/* If it failed, remove the mapping and exit, returning the heztex_torgba32 code */
	if (decstat != ST_NO_ERROR) {
		umunmap(mapped, size);
		return decstat;
	}

	/* Set the neccessary unmapping details */
	output->free_address = mapped;
	output->munmap_size = size;
	output->mmapped = true;

	return ST_NO_ERROR;
}

STATUS hztex_free_surf(struct hztex_loaded *output)
{
	if (!output->free_address) return ST_NULL_FED;

	if (output->mmapped) {
		if (umunmap(output->free_address, output->munmap_size) != 0) return ST_MAP_FAILURE;
		return ST_NO_ERROR;
	}

	free(output->free_address);
	return ST_NO_ERROR;
}

STATUS hztex_load_file(CHR *filename, struct hztex_loaded *output)
{
	/* Open the file */
	FILE *fp = fopen(filename, "r");
	if (!fp) return ST_FILE_FAILURE;

	/* Get the size of the file by seeking to the end and then back to the beginning again */
	if (fseek(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return ST_FILE_FAILURE;
	}
	SX size_boundary = ftell(fp);
	rewind(fp);

	STATUS decstat = hztex_fd_torgba32(fileno(fp), size_boundary, 0, output);
	fclose(fp);

	return decstat;
}
