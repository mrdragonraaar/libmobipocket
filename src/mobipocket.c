/**
 * mobipocket.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "mobipocket.h"

/* Default MOBIPocket */
const mobipocket_t _mobipocket = { };

size_t get_full_name(char **full_name, const mobi_header_t mobi_h,
   const char *pdb_record_0);

uint16_t mobipocket_cover_index(const mobipocket_t mobipocket);
uint16_t mobipocket_thumb_index(const mobipocket_t mobipocket);

/**
 * Initialise MOBIPocket.
 * @param *mobipocket pointer to location that stores MOBIPocket.
 */
void init_mobipocket(mobipocket_t *mobipocket)
{
	*mobipocket = _mobipocket;

	init_pdb(&mobipocket->pdb);
	init_palmdoc_header(&mobipocket->palmdoc_header);
	init_mobi_header(&mobipocket->mobi_header);
	init_exth_header(&mobipocket->exth_header);
}

/**
 * Read MOBIPocket from open file stream.
 * @param *mobipocket_f open file stream of MOBIPocket file.
 * @param *mobipocket pointer to location that stores MOBIPocket.
 * @return uint8_t exit status.
 */
uint8_t read_mobipocket(FILE *mobipocket_f, mobipocket_t *mobipocket)
{
	uint8_t status;

	init_mobipocket(mobipocket);

	/* Palm Database */
	if ((status = read_pdb(mobipocket_f, &mobipocket->pdb)))
	{
		free_mobipocket(mobipocket);
		return status;
	}

	/* PalmDOC Header */
	char *pdb_record_0 = mobipocket->pdb.records.record[0].data;
	if (get_palmdoc_header(&mobipocket->palmdoc_header, pdb_record_0) != 
	   PALMDOC_HEADER_LEN)
	{
		free_mobipocket(mobipocket);
		return EXIT_FAILURE_PALMDOC;
	}

	if (is_mobipocket(*mobipocket))
	{
		/* MOBI Header */
		size_t offset = get_mobi_header(&mobipocket->mobi_header, 
		   pdb_record_0);
		if (offset == -1)
		{
			free_mobipocket(mobipocket);
			return EXIT_FAILURE_MOBI;
		}

		/* EXTH Header */
		if (has_exth_header(mobipocket->mobi_header))
		{
			offset = get_exth_header(&mobipocket->exth_header,
			   offset, pdb_record_0);
			if (offset == -1)
			{
				free_mobipocket(mobipocket);
				return EXIT_FAILURE_EXTH;
			}
		}

		/* Full Name */
		get_full_name(&mobipocket->full_name, mobipocket->mobi_header, 
		   pdb_record_0);
	}

	return EXIT_SUCCESS;
}

/**
 * Free memory allocated to MOBIPocket.
 * @param *mobipocket pointer to location that stores MOBIPocket.
 */
void free_mobipocket(mobipocket_t *mobipocket)
{
	free_exth_header(&mobipocket->exth_header);
	free_pdb(&mobipocket->pdb);

	if (mobipocket->full_name != NULL)
		free(mobipocket->full_name);

	init_mobipocket(mobipocket);
}

/**
 * Get full name from PDB record 0.
 * @param *full_name full name string.
 * @param mobi_h MOBI header.
 * @param *pdb_record_0 pointer to PDB record 0.
 * @return size_t new offset in PDB record 0.
 */
size_t get_full_name(char **full_name, const mobi_header_t mobi_h,
   const char *pdb_record_0)
{
	return get_header_field_str(full_name, mobi_h.full_name_length, 
	   mobi_h.full_name_offset, pdb_record_0);
}

/**
 * Check type and creator of Palm Database is for MOBIPocket.
 * @param mobipocket MOBIPocket.
 * @return uint8_t non-zero if type and creator match MOBIPocket.
 */
uint8_t is_mobipocket(const mobipocket_t mobipocket)
{
	return is_pdb_type(mobipocket.pdb, PDB_TYPE_MOBI, PDB_CREATOR_MOBI);
}

/**
 * Get the cover image data.
 * @param mobipocket MOBIPocket.
 * @return char* cover image data.
 */
char* mobipocket_cover(const mobipocket_t mobipocket)
{
	return mobipocket_image_record(mobipocket, 
	   mobipocket_cover_index(mobipocket));
}

/**
 * Get the length of the cover image data.
 * @param mobipocket MOBIPocket.
 * @return uint32_t cover image data length.
 */
uint32_t mobipocket_cover_length(const mobipocket_t mobipocket)
{
	return mobipocket_image_record_length(mobipocket, 
	   mobipocket_cover_index(mobipocket));
}

/**
 * Get the index of the cover image data in the Palm Database records.
 * @param mobipocket MOBIPocket.
 * @return uint16_t cover image data index.
 */
uint16_t mobipocket_cover_index(const mobipocket_t mobipocket)
{
	return mobipocket_image_record_offset2index(mobipocket, 
	   mobipocket_cover_offset(mobipocket));
}

/**
 * Get the thumbnail image data.
 * @param mobipocket MOBIPocket.
 * @return char* thumbnail image data.
 */
char* mobipocket_thumb(const mobipocket_t mobipocket)
{
	return mobipocket_image_record(mobipocket, 
	   mobipocket_thumb_index(mobipocket));
}

/**
 * Get the length of the thumbnail image data.
 * @param mobipocket MOBIPocket.
 * @return uint32_t thumbnail image data length.
 */
uint32_t mobipocket_thumb_length(const mobipocket_t mobipocket)
{
	return mobipocket_image_record_length(mobipocket, 
	   mobipocket_thumb_index(mobipocket));
}

/**
 * Get the index of the thumbnail image data in the Palm Database records.
 * @param mobipocket MOBIPocket.
 * @return uint16_t thumbnail image data index.
 */
uint16_t mobipocket_thumb_index(const mobipocket_t mobipocket)
{
	return mobipocket_image_record_offset2index(mobipocket, 
	   mobipocket_thumb_offset(mobipocket));
}

/**
 * Get the data of the image record at specified index.
 * @param mobipocket MOBIPocket.
 * @param index index of image record.
 * @return char* image data.
 */
char* mobipocket_image_record(const mobipocket_t mobipocket, uint16_t index)
{
	/* Not a image record */
	if (!mobipocket_image_record_length(mobipocket, index))
		return NULL;

	return mobipocket.pdb.records.record[index].data;
}

/**
 * Get the length of the image record at specified index.
 * @param mobipocket MOBIPocket.
 * @param index index of image record.
 * @return uint32_t image record data length.
 */
uint32_t mobipocket_image_record_length(const mobipocket_t mobipocket, 
   uint16_t index)
{
	/* Not a image record */
	if (!is_mobipocket_image_record_index(mobipocket, index))
		return 0;

	return get_pdb_record_data_len(mobipocket.pdb, index);
}

/**
 * Get the index of image in the Palm Database records from offset.
 * @param mobipocket MOBIPocket.
 * @param offset offset of image record.
 * @return uint16_t image data index.
 */
uint16_t mobipocket_image_record_offset2index(const mobipocket_t mobipocket, 
   uint16_t offset)
{
	uint16_t index = mobipocket.mobi_header.first_image_index + offset;
	if (is_mobipocket_image_record_index(mobipocket, index))
		return index;

	return 0;
}

/**
 * Check if specified index is a valid image record index.
 * @param mobipocket MOBIPocket.
 * @param index index of image record.
 * @return uint8_t non-zero if valid image record index.
 */
uint8_t is_mobipocket_image_record_index(const mobipocket_t mobipocket, 
   uint16_t index)
{
	uint16_t start_index = mobipocket.mobi_header.first_image_index;
	uint16_t end_index = mobipocket.mobi_header.last_content_index;

	return ((start_index > 0) && (end_index >= start_index) && 
	   (index >= start_index) && (index <= end_index));
}

/**
 * Get the MOBIPocket text.
 * @param *text text data.
 * @param mobipocket MOBIPocket.
 * @return uint32_t text data length.
 */
uint32_t mobipocket_text(char *text, const mobipocket_t mobipocket)
{
	uint32_t text_length = 0;
	*text = '\0';

	/* Encryption */
	if (!is_palmdoc_encryption_none(mobipocket.palmdoc_header))
		return 0;

	/* No Encryption */
	uint16_t start_index = mobipocket.mobi_header.first_content_index;
	uint16_t end_index = mobipocket.mobi_header.first_non_book_index - 1;
	if ((start_index <= 0) || (end_index < start_index))
		return 0;

	uint16_t index;
	for (index = start_index; index <= end_index; index++)
	{
		uint32_t len = mobipocket_text_record_length(mobipocket, index);
		if (len)
		{
			char text_record[len];
			mobipocket_text_record(text_record, mobipocket, index);
			strncat(text, text_record, len);
			text_length += len;
		}
	}

	return text_length;
}

/**
 * Get the length of the MOBIPocket text.
 * @param mobipocket MOBIPocket.
 * @return uint32_t text data length.
 */
uint32_t mobipocket_text_length(const mobipocket_t mobipocket)
{
	uint32_t text_length = 0;

	/* Encryption */
	if (!is_palmdoc_encryption_none(mobipocket.palmdoc_header))
		return 0;

	/* No Encryption */
	uint16_t start_index = mobipocket.mobi_header.first_content_index;
	uint16_t end_index = mobipocket.mobi_header.first_non_book_index - 1;
	if ((start_index <= 0) || (end_index < start_index))
		return 0;

	uint16_t index;
	for (index = start_index; index <= end_index; index++)
		text_length += mobipocket_text_record_length(mobipocket, index);

	return text_length;
}

/**
 * Get the data of the text record at specified index.
 * @param *text text record data.
 * @param mobipocket MOBIPocket.
 * @param index index of text record.
 * @return uint32_t text record data length.
 */
uint32_t mobipocket_text_record(char *text, const mobipocket_t mobipocket, 
   uint16_t index)
{
	*text = '\0';

	/* Not a text record */
	if (!is_mobipocket_text_record_index(mobipocket, index))
		return 0;

	uint32_t len = get_pdb_record_data_len(mobipocket.pdb, index);
	if (len > 0)
	{
		char *text_compress = mobipocket.pdb.records.record[index].data;

		/* No Compression */
		if (is_palmdoc_compression_none(mobipocket.palmdoc_header))
		{
			text = text_compress;
			return len;
		}

		/* Assume PalmDoc (LZ77) Compression */
		if (text_compress != NULL)
			return lz77_decode(text, text_compress, len);
	}

	return 0;
}

/**
 * Get the length of the text record at specified index.
 * @param mobipocket MOBIPocket.
 * @param index index of text record.
 * @return uint32_t text record data length.
 */
uint32_t mobipocket_text_record_length(const mobipocket_t mobipocket, 
   uint16_t index)
{
	/* Not a text record */
	if (!is_mobipocket_text_record_index(mobipocket, index))
		return 0;

	uint32_t len = get_pdb_record_data_len(mobipocket.pdb, index);
	if (len > 0)
	{
		char *text = mobipocket.pdb.records.record[index].data;

		/* No Compression */
		if (is_palmdoc_compression_none(mobipocket.palmdoc_header))
			return len;

		/* Assume PalmDoc (LZ77) Compression */
		return lz77_decode_length(text, len);
	}

	return 0;
}

/**
 * Get the index of text record in the Palm Database records from offset.
 * @param mobipocket MOBIPocket.
 * @param offset offset of text record.
 * @return uint16_t text data index.
 */
uint16_t mobipocket_text_record_offset2index(const mobipocket_t mobipocket, 
   uint16_t offset)
{
	uint16_t index = mobipocket.mobi_header.first_content_index + offset;
	if (is_mobipocket_text_record_index(mobipocket, index))
		return index;

	return 0;
}

/**
 * Check if specified index is a valid text record index.
 * @param mobipocket MOBIPocket.
 * @param index index of text record.
 * @return uint8_t non-zero if valid text record index.
 */
uint8_t is_mobipocket_text_record_index(const mobipocket_t mobipocket, 
   uint16_t index)
{
	uint16_t start_index = mobipocket.mobi_header.first_content_index;
	uint16_t end_index = mobipocket.mobi_header.first_non_book_index - 1;

	return ((start_index > 0) && (end_index >= start_index) && 
	   (index >= start_index) && (index <= end_index));
}

/**
 * Get the title.
 * Returns full name if updated title does not exist.
 * @param mobipocket MOBIPocket.
 * @return char* title.
 */
char* mobipocket_title(const mobipocket_t mobipocket)
{
	if (mobipocket_updated_title(mobipocket) != NULL)
		return mobipocket_updated_title(mobipocket);

	return mobipocket_full_name(mobipocket);
}

