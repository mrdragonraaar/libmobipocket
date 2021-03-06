/**
 * mobi_header.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "mobi_header.h"

/* Default MOBI Header */
const mobi_header_t _mobi_header = { };

/**
 * Initialise MOBI header.
 * @param *mobi_h pointer to location that stores MOBI header.
 */
void init_mobi_header(mobi_header_t *mobi_h)
{
	*mobi_h = _mobi_header;
}

/**
 * Get MOBI header from PDB record 0.
 * @param *mobi_h pointer to location that stores MOBI header.
 * @param *pdb_record_0 pointer to PDB record 0.
 * @return size_t offset in PDB record 0.
 */
size_t get_mobi_header(mobi_header_t *mobi_h, const char *pdb_record_0)
{
	base_header_t base_h;
	size_t offset = get_base_header(&base_h, PALMDOC_HEADER_LEN, 
	   pdb_record_0);

	if (offset != -1)
	{
		if (is_mobi_header(base_h))
			convert_base2mobi(mobi_h, base_h);
		else
			offset = -1;

		free_base_header(&base_h);
	}

	return offset;
}

/**
 * Convert base header to MOBI header.
 * @param *mobi_h pointer to location that stores MOBI header.
 * @param base_h base header.
 */
void convert_base2mobi(mobi_header_t *mobi_h, const base_header_t base_h)
{
	init_mobi_header(mobi_h);

	/* Identifier */
	memcpy(&mobi_h->identifier, base_h.identifier, 
	   sizeof(mobi_h->identifier));
	/* Header Length */
	mobi_h->header_length = base_h.header_length;

	size_t offset = 0;
	size_t data_length = mobi_h->header_length - BASE_HEADER_LEN;

	/* MOBI Type */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->mobi_type, offset, base_h.data);
	/* Text Encoding */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->text_encoding, offset, 
	   base_h.data);
	/* Unique ID */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->unique_id, offset, base_h.data);
	/* File Version */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->file_version, offset, base_h.data);
	/* Ortographic Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->ortographic_index, offset, 
	   base_h.data);
	/* Inflection Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->inflection_index, offset, 
	   base_h.data);
	/* Index Names */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->index_names, offset, base_h.data);
	/* Index Keys */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->index_keys, offset, base_h.data);
	/* Extra Index 0 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_0, offset, 
	   base_h.data);
	/* Extra Index 1 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_1, offset, 
	   base_h.data);
	/* Extra Index 2 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_2, offset, 
	   base_h.data);
	/* Extra Index 3 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_3, offset, 
	   base_h.data);
	/* Extra Index 4 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_4, offset, 
	   base_h.data);
	/* Extra Index 5 */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_index_5, offset, 
	   base_h.data);
	/* First Non-Book Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->first_non_book_index, offset, 
	   base_h.data);
	/* Full Name Offset */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->full_name_offset, offset, 
	   base_h.data);
	/* Full Name Length */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->full_name_length, offset, 
	   base_h.data);
	/* Locale */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->locale, offset, base_h.data);
	/* Input Language */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->input_language, offset, 
	   base_h.data);
	/* Output Language */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->output_language, offset, 
	   base_h.data);
	/* Min Version */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->min_version, offset, base_h.data);
	/* First Image Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->first_image_index, offset, 
	   base_h.data);
	/* Huffman Record Offset */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->huffman_record_offset, offset, 
	   base_h.data);
	/* Huffman Record Count */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->huffman_record_count, offset, 
	   base_h.data);
	/* Huffman Table Offset */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->huffman_table_offset, offset, 
	   base_h.data);
	/* Huffman Table Length */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->huffman_table_length, offset, 
	   base_h.data);
	/* EXTH Flags */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->exth_flags, offset, base_h.data);
	/* ignore unknown bytes */
	if (offset + 32 > data_length) return;
	offset += 32;
	/* DRM Offset */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->drm_offset, offset, base_h.data);
	/* DRM Count */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->drm_count, offset, base_h.data);
	/* DRM Size */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->drm_size, offset, base_h.data);
	/* DRM Flags */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->drm_flags, offset, base_h.data);
	/* ignore unknown bytes */
	if (offset + 12 > data_length) return;
	offset += 12;
	/* First Content Record Index */
	if (offset + 2 > data_length) return;
	offset = get_header_field_s(&mobi_h->first_content_index, offset, 
	   base_h.data);
	/* Last Content Record Index */
	if (offset + 2 > data_length) return;
	offset = get_header_field_s(&mobi_h->last_content_index, offset, 
	   base_h.data);
	/* ignore unknown bytes */
	if (offset + 4 > data_length) return;
	offset += 4;
	/* FCIS Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->fcis_index, offset, base_h.data);
	/* ignore unknown bytes */
	if (offset + 4 > data_length) return;
	offset += 4;
	/* FLIS Index */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->flis_index, offset, base_h.data);
	/* ignore unknown bytes */
	if (offset + 4 > data_length) return;
	offset += 4;
	/* ignore unknown bytes */
	if (offset + 8 > data_length) return;
	offset += 8;
	/* ignore unknown bytes */
	if (offset + 4 > data_length) return;
	offset += 4;
	/* First Compilation Data Section Count */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(
	   &mobi_h->first_compilation_data_section_count, offset, base_h.data);
	/* Number of Compilation Data Sections */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(
	   &mobi_h->number_of_compilation_data_sections, offset, base_h.data);
	/* ignore unknown bytes */
	if (offset + 4 > data_length) return;
	offset += 4;
	/* Extra Record Data Flags */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->extra_record_data_flags, offset, 
	   base_h.data);
	/* INDX Record Offset */
	if (offset + 4 > data_length) return;
	offset = get_header_field_l(&mobi_h->indx_offset, offset, base_h.data);
}

/**
 * Get MOBI type as string.
 * @param mobi_h MOBI header.
 * @return char* MOBI type string.
 */
char* mobi_type_str(const mobi_header_t mobi_h)
{
	switch (mobi_h.mobi_type)
	{
		case MOBI_TYPE_MOBIPOCKET:
			return MOBI_TYPE_MOBIPOCKET_STR;
		case MOBI_TYPE_PALMDOC:
			return MOBI_TYPE_PALMDOC_STR;
		case MOBI_TYPE_AUDIO:
			return MOBI_TYPE_AUDIO_STR;
		case MOBI_TYPE_KINDLEGEN:
			return MOBI_TYPE_KINDLEGEN_STR;
		case MOBI_TYPE_KF8:
			return MOBI_TYPE_KF8_STR;
		case MOBI_TYPE_NEWS:
			return MOBI_TYPE_NEWS_STR;
		case MOBI_TYPE_NEWSFEED:
			return MOBI_TYPE_NEWSFEED_STR;
		case MOBI_TYPE_MAGAZINE:
			return MOBI_TYPE_MAGAZINE_STR;
		case MOBI_TYPE_PICS:
			return MOBI_TYPE_PICS_STR;
		case MOBI_TYPE_WORD:
			return MOBI_TYPE_WORD_STR;
		case MOBI_TYPE_XLS:
			return MOBI_TYPE_XLS_STR;
		case MOBI_TYPE_PPT:
			return MOBI_TYPE_PPT_STR;
		case MOBI_TYPE_TEXT:
			return MOBI_TYPE_TEXT_STR;
		case MOBI_TYPE_HTML:
			return MOBI_TYPE_HTML_STR;
		default:
			return NULL;
	}
}

/**
 * Get MOBI text encoding as string.
 * @param mobi_h MOBI header.
 * @return char* MOBI text encoding string.
 */
char* mobi_text_encoding_str(const mobi_header_t mobi_h)
{
	switch (mobi_h.text_encoding)
	{
		case MOBI_ENCODING_WINLATIN1:
			return MOBI_ENCODING_WINLATIN1_STR;
		case MOBI_ENCODING_UTF8:
			return MOBI_ENCODING_UTF8_STR;
		default:
			return NULL;
	}
}

/**
 * Check if EXTH header should exist.
 * @param mobi_h MOBI header.
 * @return uint8_t non-zero if EXTH header should exist.
 */
uint8_t has_exth_header(const mobi_header_t mobi_h)
{
	return mobi_h.exth_flags & 0x40;
}

/**
 * Print MOBI header.
 * @param mobi_h MOBI header.
 */
void print_mobi_header(const mobi_header_t mobi_h)
{
	printf("Identifier: %s\n", mobi_h.identifier);
	printf("Header Length: %i\n", mobi_h.header_length);
	printf("MOBI Type: %i\n", mobi_h.mobi_type);
	printf("Text Encoding: %i\n", mobi_h.text_encoding);
	printf("Unique ID: %i\n", mobi_h.unique_id);
	printf("File Version: %i\n", mobi_h.file_version);
	printf("Ortographic Index: %i\n", mobi_h.ortographic_index);
	printf("Inflection Index: %i\n", mobi_h.inflection_index);
	printf("Index Names: %i\n", mobi_h.index_names);
	printf("Index Keys: %i\n", mobi_h.index_keys);
	printf("Extra Index 0: %i\n", mobi_h.extra_index_0);
	printf("Extra Index 1: %i\n", mobi_h.extra_index_1);
	printf("Extra Index 2: %i\n", mobi_h.extra_index_2);
	printf("Extra Index 3: %i\n", mobi_h.extra_index_3);
	printf("Extra Index 4: %i\n", mobi_h.extra_index_4);
	printf("Extra Index 5: %i\n", mobi_h.extra_index_5);
	printf("First Non-Book Index: %i\n", mobi_h.first_non_book_index);
	printf("Full Name Offset: %i\n", mobi_h.full_name_offset);
	printf("Full Name Length: %i\n", mobi_h.full_name_length);
	printf("Locale: %i\n", mobi_h.locale);
	printf("Input Language: %i\n", mobi_h.input_language);
	printf("Output Language: %i\n", mobi_h.output_language);
	printf("Min Version: %i\n", mobi_h.min_version);
	printf("First Image Index: %i\n", mobi_h.first_image_index);
	printf("Huffman Record Offset: %i\n", mobi_h.huffman_record_offset);
	printf("Huffman Record Count: %i\n", mobi_h.huffman_record_count);
	printf("Huffman Table Offset: %i\n", mobi_h.huffman_table_offset);
	printf("Huffman Table Length: %i\n", mobi_h.huffman_table_length);
	printf("EXTH Flags: %i\n", mobi_h.exth_flags);
	printf("DRM Offset: %i\n", mobi_h.drm_offset);
	printf("DRM Count: %i\n", mobi_h.drm_count);
	printf("DRM Size: %i\n", mobi_h.drm_size);
	printf("DRM Flags: %i\n", mobi_h.drm_flags);
	printf("First Content Index: %i\n", mobi_h.first_content_index);
	printf("Last Content Index: %i\n", mobi_h.last_content_index);
	printf("FCIS Index: %i\n", mobi_h.fcis_index);
	printf("FLIS Index: %i\n", mobi_h.flis_index);
	printf("First Compilation Data Section Count: %i\n", 
	   mobi_h.first_compilation_data_section_count);
	printf("Number of Compilation Data Sections: %i\n", 
	   mobi_h.number_of_compilation_data_sections);
	printf("Extra Record Data Flags: %i\n", mobi_h.extra_record_data_flags);
	printf("INDX Record Offset: %i\n", mobi_h.indx_offset);
}

