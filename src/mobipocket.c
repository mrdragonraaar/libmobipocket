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
	uint16_t cover_index = mobipocket_cover_index(mobipocket);
	if (cover_index && mobipocket_cover_length(mobipocket))
		return mobipocket.pdb.records.record[cover_index].data;

	return NULL;
}

/**
 * Get the length of the cover image data.
 * @param mobipocket MOBIPocket.
 * @return uint32_t cover image data length.
 */
uint32_t mobipocket_cover_length(const mobipocket_t mobipocket)
{
	uint16_t cover_index = mobipocket_cover_index(mobipocket);
	if (cover_index)
		return get_pdb_record_data_len(mobipocket.pdb, cover_index);

	return 0;
}

/**
 * Get the index of the cover image data in the Palm Database records.
 * @param mobipocket MOBIPocket.
 * @return uint16_t cover image data index.
 */
uint16_t mobipocket_cover_index(const mobipocket_t mobipocket)
{
	return mobipocket.mobi_header.first_image_index +
	   mobipocket_cover_offset(mobipocket);
}

/**
 * Get the thumbnail image data.
 * @param mobipocket MOBIPocket.
 * @return char* thumbnail image data.
 */
char* mobipocket_thumb(const mobipocket_t mobipocket)
{
	uint16_t thumb_index = mobipocket_thumb_index(mobipocket);
	if (thumb_index && mobipocket_thumb_length(mobipocket))
		return mobipocket.pdb.records.record[thumb_index].data;

	return NULL;
}

/**
 * Get the length of the thumbnail image data.
 * @param mobipocket MOBIPocket.
 * @return uint32_t thumbnail image data length.
 */
uint32_t mobipocket_thumb_length(const mobipocket_t mobipocket)
{
	uint16_t thumb_index = mobipocket_thumb_index(mobipocket);
	if (thumb_index)
		return get_pdb_record_data_len(mobipocket.pdb, thumb_index);

	return 0;
}

/**
 * Get the index of the thumbnail image data in the Palm Database records.
 * @param mobipocket MOBIPocket.
 * @return uint16_t thumbnail image data index.
 */
uint16_t mobipocket_thumb_index(const mobipocket_t mobipocket)
{
	return mobipocket.mobi_header.first_image_index +
	   mobipocket_thumb_offset(mobipocket);
}

