/**
 * palmdoc_header.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "palmdoc_header.h"

/* Default PalmDOC Header */
const palmdoc_header_t _palmdoc_header = { };

uint8_t valid_palmdoc_compression(const palmdoc_header_t palmdoc_h);
uint8_t valid_palmdoc_encryption(const palmdoc_header_t palmdoc_h);
uint8_t valid_palmdoc(const palmdoc_header_t palmdoc_h);

/**
 * Initialise PalmDOC header.
 * @param *palmdoc_h pointer to location that stores PalmDOC header.
 */
void init_palmdoc_header(palmdoc_header_t *palmdoc_h)
{
	*palmdoc_h = _palmdoc_header;
}

/**
 * Get PalmDOC header from PDB record 0.
 * @param *palmdoc_h pointer to location that stores PalmDOC header.
 * @param *pdb_record_0 pointer to PDB record 0.
 * @return size_t offset in PDB record 0 (should be 16 if successful).
 */
size_t get_palmdoc_header(palmdoc_header_t *palmdoc_h, char *pdb_record_0)
{
	char header[PALMDOC_HEADER_LEN];
	size_t header_offset = 0;
	size_t offset = 0;

	init_palmdoc_header(palmdoc_h);

	offset = get_header_field(header, sizeof(header), offset, 
	   pdb_record_0);

	/* Compression */
	header_offset = get_header_field_s(&palmdoc_h->compression, 
	   header_offset, header);
	/* Unused */
	header_offset = get_header_field_s(&palmdoc_h->unused, header_offset, 
	   header);
	/* Text Length */
	header_offset = get_header_field_l(&palmdoc_h->text_length, 
	   header_offset, header);
	/* Record Count */
	header_offset = get_header_field_s(&palmdoc_h->record_count, 
	   header_offset, header);
	/* Record Size */
	header_offset = get_header_field_s(&palmdoc_h->record_size, 
	   header_offset, header);
	/* Encryption Type */
	header_offset = get_header_field_s(&palmdoc_h->encryption_type, 
	   header_offset, header);
	/* Unknown */
	header_offset = get_header_field_s(&palmdoc_h->unknown, header_offset,
	   header);

	if (valid_palmdoc(*palmdoc_h))
		return offset;

	return -1;
}

/**
 * Check is PalmDOC header compression matches given compression value.
 * @param palmdoc_h PalmDOC header.
 * @param compression compression value.
 * @return uint16_t non-zero if compression values match.
 */
uint8_t is_palmdoc_compression(const palmdoc_header_t palmdoc_h, 
   uint16_t compression)
{
	return palmdoc_h.compression == compression;
}

/**
 * Check is PalmDOC header compression is valid.
 * @param palmdoc_h PalmDOC header.
 * @return uint16_t non-zero if compression is valid.
 */
uint8_t valid_palmdoc_compression(const palmdoc_header_t palmdoc_h)
{
	return (is_palmdoc_compression_none(palmdoc_h) || 
	   is_palmdoc_compression_palmdoc(palmdoc_h) || 
	   is_palmdoc_compression_huff_cdic(palmdoc_h));
}

/**
 * Check is PalmDOC header encryption type matches given encryption type value.
 * @param palmdoc_h PalmDOC header.
 * @param encryption_type encryption type value.
 * @return uint16_t non-zero if encryption type values match.
 */
uint8_t is_palmdoc_encryption(const palmdoc_header_t palmdoc_h, 
   uint16_t encryption_type)
{
	return palmdoc_h.encryption_type == encryption_type;
}

/**
 * Check is PalmDOC header encryption type is valid.
 * @param palmdoc_h PalmDOC header.
 * @return uint16_t non-zero if encryption type is valid.
 */
uint8_t valid_palmdoc_encryption(const palmdoc_header_t palmdoc_h)
{
	return (is_palmdoc_encryption_none(palmdoc_h) || 
	   is_palmdoc_encryption_mobi_old(palmdoc_h) || 
	   is_palmdoc_encryption_mobi(palmdoc_h));
}

/**
 * Check is PalmDOC header is valid.
 * @param palmdoc_h PalmDOC header.
 * @return uint16_t non-zero if valid.
 */
uint8_t valid_palmdoc(const palmdoc_header_t palmdoc_h)
{
	return ((palmdoc_h.record_count != 0) &&
	   valid_palmdoc_encryption(palmdoc_h) && 
	   valid_palmdoc_compression(palmdoc_h));
}

/**
 * Print PalmDOC header information.
 * @param palmdoc_h PalmDOC header.
 */
void print_palmdoc_header(const palmdoc_header_t palmdoc_h)
{
	printf("Compression: %i\n", palmdoc_h.compression);
	printf("Unused: %i\n", palmdoc_h.unused);
	printf("Text Length: %i\n", palmdoc_h.text_length);
	printf("Record Count: %i\n", palmdoc_h.record_count);
	printf("Record Size: %i\n", palmdoc_h.record_size);
	printf("Encryption: %i\n", palmdoc_h.encryption_type);
	printf("Unknown: %i\n", palmdoc_h.unknown);
}

