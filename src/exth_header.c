/**
 * exth_header.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "exth_header.h"

/* Default EXTH Header */
const exth_header_t _exth_header = { };
/* Default EXTH Record */
const exth_record_t _exth_record = { };

size_t get_exth_records(exth_header_t *exth_h, size_t offset, 
   const char *exth_rec_buf);
void init_exth_record(exth_record_t *exth_r);
size_t get_exth_record(exth_record_t *exth_r, size_t offset, 
   const char *exth_rec_buf);

/**
 * Initialise EXTH header.
 * @param *exth_h pointer to location that stores EXTH header.
 */
void init_exth_header(exth_header_t *exth_h)
{
	*exth_h = _exth_header;
}

/**
 * Get EXTH header from PDB record 0.
 * @param *exth_h pointer to location that stores EXTH header.
 * @param offset offset in PDB record 0.
 * @param *pdb_record_0 pointer to PDB record 0.
 * @return size_t new offset in PDB record 0.
 */
size_t get_exth_header(exth_header_t *exth_h, size_t offset, 
   const char *pdb_record_0)
{
	base_header_t base_h;

	offset = get_base_header(&base_h, offset, pdb_record_0);

	if (offset != 1)
	{
		if (is_exth_header(base_h))
		{
			convert_base2exth(exth_h, base_h);

			/* add padding to offset (multiple of 4 bytes) */
			offset += 4 - (exth_h->header_length % 4);
		}
		else
			offset = -1;

		free_base_header(&base_h);
	}

	return offset;
}

/**
 * Convert base header to EXTH header.
 * @param *exth_h pointer to location that stores EXTH header.
 * @param *base_h base header.
 */
void convert_base2exth(exth_header_t *exth_h, const base_header_t base_h)
{
	init_exth_header(exth_h);

	/* Identifier */
	memcpy(&exth_h->identifier, base_h.identifier, 
	   sizeof(exth_h->identifier));
	/* Header Length */
	exth_h->header_length = base_h.header_length;

	size_t offset = 0;
	/* Record Count */
	offset = get_header_field_l(&exth_h->record_count, offset, base_h.data);

	/* Records */
	get_exth_records(exth_h, offset, base_h.data);
}

/**
 * Get EXTH records from EXTH record buffer.
 * @param *exth_h pointer to location that stores EXTH header.
 * @param offset offset in EXTH record buffer.
 * @param *exth_rec_buf pointer to EXTH record buffer.
 * @return size_t new offset in EXTH record buffer.
 */
size_t get_exth_records(exth_header_t *exth_h, size_t offset, 
   const char *exth_rec_buf)
{
	uint32_t record_count = exth_h->record_count;

	if (record_count)
	{
		exth_h->record = malloc(record_count * sizeof(exth_record_t));

		uint32_t i;
		for (i = 0; i < record_count; i++)
		{
			offset = get_exth_record(&exth_h->record[i], offset,
			   exth_rec_buf);
		}
	}

	return offset;
}

/**
 * Initialise EXTH record.
 * @param *exth_r pointer to location that stores EXTH record.
 */
void init_exth_record(exth_record_t *exth_r)
{
	*exth_r = _exth_record;
}

/**
 * Get a single EXTH record from EXTH record buffer.
 * @param *exth_r pointer to location that stores EXTH record.
 * @param offset offset in EXTH record buffer.
 * @param *exth_rec_buf pointer to EXTH record buffer.
 * @return size_t new offset in EXTH record buffer.
 */
size_t get_exth_record(exth_record_t *exth_r, size_t offset, 
   const char *exth_rec_buf)
{
	init_exth_record(exth_r);

	/* Record Type */
	offset = get_header_field_l(&exth_r->record_type, offset, exth_rec_buf);
	/* Record Length */
	offset = get_header_field_l(&exth_r->record_length, offset, 
	   exth_rec_buf);

	if (exth_r->record_length)
	{
		size_t data_length = exth_r->record_length - 
		   EXTH_RECORD_HEADER_LEN;

		/* Record Data */
		offset = get_header_field_str(&exth_r->record_data, 
		   data_length, offset, exth_rec_buf);
	}

	return offset;
}

/**
 * Free memory allocated to EXTH header.
 * @param *exth_h pointer to location that stores EXTH header.
 */
void free_exth_header(exth_header_t *exth_h)
{
	if (exth_h->record != NULL)
	{
		uint32_t record_count = exth_h->record_count;
		uint32_t i;
		for (i = 0; i < record_count; i++)
		{
			if (exth_h->record[i].record_data != NULL)
				free(exth_h->record[i].record_data);
		}

		free(exth_h->record);
	}

	init_exth_header(exth_h);
}

/**
 * Get EXTH record data of first record from EXTH records array
 *    that matches record type.
 * @param exth_h EXTH header.
 * @param record_type EXTH record type.
 * @return char* record data string.
 */
char* exth_record(const exth_header_t exth_h, uint32_t record_type)
{
	char *record_data = NULL;
	exth_record_data(&record_data, exth_h, 0, record_type);

	return record_data;
}

/**
 * Get EXTH record data as uint32 of first record from EXTH records array
 *    that matches record type.
 * @param exth_h EXTH header.
 * @param record_type EXTH record type.
 * @return uint32_t record data uint32.
 */
uint32_t exth_record_l(const exth_header_t exth_h, uint32_t record_type)
{
	uint32_t record_data = 0;
	get_header_field_l(&record_data, 0, exth_record(exth_h, record_type));

	return record_data;
}

/**
 * Get EXTH record data from EXTH records array for specific 
 *    record type starting at offset.
 * @param **record_data pointer to location of record data.
 * @param exth_h EXTH header.
 * @param offset_index offset in EXTH records array.
 * @param record_type EXTH record type.
 * @return size_t index in EXTH records array.
 */
uint32_t exth_record_data(char **record_data, const exth_header_t exth_h, 
   uint32_t offset_index, uint32_t record_type)
{
	*record_data = NULL;
	offset_index = exth_record_index(exth_h, offset_index, record_type);

	if (offset_index != -1)
		*record_data = exth_h.record[offset_index].record_data;

	return offset_index;
}

/**
 * Get EXTH record index from EXTH records array for specific 
 *    record type starting at offset.
 * @param exth_h EXTH header.
 * @param offset_index offset in EXTH records array.
 * @param record_type EXTH record type.
 * @return size_t index in EXTH records array.
 */
uint32_t exth_record_index(const exth_header_t exth_h, 
   uint32_t offset_index, uint32_t record_type)
{
	uint32_t index;
	for (index = offset_index; index < exth_h.record_count; index++)
	{
		if (exth_h.record != NULL &&
		   exth_h.record[index].record_type == record_type)
			return index;
	}

	return -1;
}

/**
 * Get creator software EXTH record as string.
 * @param exth_h EXTH header.
 * @return char* creator software string.
 */
char* exth_creator_software_str(const exth_header_t exth_h)
{
	switch(exth_record_l(exth_h, EXTH_RECORD_TYPE_CREATORSOFTWARE))
	{
		case CREATORSOFTWARE_MOBIGEN: 
			return CREATORSOFTWARE_MOBIGEN_STR;
		case CREATORSOFTWARE_MOBIPOCKETCREATOR: 
			return CREATORSOFTWARE_MOBIPOCKETCREATOR_STR;
		case CREATORSOFTWARE_KINDLEGEN_WINDOWS:
			return CREATORSOFTWARE_KINDLEGEN_WINDOWS_STR;
		case CREATORSOFTWARE_KINDLEGEN_LINUX:
			return CREATORSOFTWARE_KINDLEGEN_LINUX_STR;
		case CREATORSOFTWARE_KINDLEGEN_MAC:
			return CREATORSOFTWARE_KINDLEGEN_MAC_STR;
		default:
			return NULL;
	}
}

/**
 * Check CDE type EXTH record matches specified CDE type.
 * @param exth_h EXTH header.
 * @param *cde_type CDE type string.
 * @return uint8_t non-zero if CDE type matches.
 */
uint8_t is_exth_cde_type(const exth_header_t exth_h, const char *cde_type)
{
	char *exth_cde_type = exth_record(exth_h, EXTH_RECORD_TYPE_CDETYPE);

	if ((exth_cde_type != NULL) && 
	   !memcmp(exth_cde_type, cde_type, (sizeof(char) * 4)))
		return 1;

	return 0;
}

/**
 * Get CDE type EXTH record as string.
 * @param exth_h EXTH header.
 * @return char* CDE type string.
 */
char* exth_cde_type_str(const exth_header_t exth_h)
{
	if (is_exth_cde_type(exth_h, CDETYPE_PERSONALDOC))
		return CDETYPE_PERSONALDOC_STR;

	if (is_exth_cde_type(exth_h, CDETYPE_EBOOK))
		return CDETYPE_EBOOK_STR;

	if (is_exth_cde_type(exth_h, CDETYPE_EBOOKSAMPLE))
		return CDETYPE_EBOOKSAMPLE_STR;

	return NULL;
}

/**
 * Print EXTH header.
 * @param exth_h EXTH header.
 */
void print_exth_header(const exth_header_t exth_h)
{
	printf("Identifier: %s\n", exth_h.identifier);
	printf("Header Length: %i\n", exth_h.header_length);
	printf("Record Count: %i\n", exth_h.record_count);

	uint32_t i;
	for (i = 0; i < exth_h.record_count; i++)
	{
		printf("Record %i: Type: %i Length: %i\n", i, 
		   exth_h.record[i].record_type, 
		   exth_h.record[i].record_length);
		printf("Record Data: %s\n", exth_h.record[i].record_data);
	}
}

