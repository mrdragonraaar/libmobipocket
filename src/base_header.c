/**
 * base_header.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "base_header.h"

/* Default Base Header */
const base_header_t _base_header = { };

uint8_t valid_base_identifier(const base_header_t base_h);
uint8_t valid_base_header_length(const base_header_t base_h);
uint8_t valid_base_header(const base_header_t base_h);

/**
 * Initialise base header.
 * @param *base_h pointer to location that stores base header.
 */
void init_base_header(base_header_t *base_h)
{
	*base_h = _base_header;
}

/**
 * Get base header from PDB record 0.
 * @param *base_h pointer to location that stores base header.
 * @param offset offset in PDB record 0.
 * @param *pdb_record_0 pointer to PDB record 0.
 * @return size_t new offset in PDB record 0.
 */
size_t get_base_header(base_header_t *base_h, size_t offset, 
   const char *pdb_record_0)
{
	init_base_header(base_h);

	/* Identifier */
	offset = get_header_field(&base_h->identifier, 
	   sizeof(base_h->identifier), offset, pdb_record_0);
	/* Header Length */
	offset = get_header_field_l(&base_h->header_length, offset, 
	   pdb_record_0);

	if (valid_base_header(*base_h))
	{
		size_t data_length = base_h->header_length - BASE_HEADER_LEN;
		base_h->data = malloc(data_length * sizeof(char));

		/* Data */
		offset = get_header_field(base_h->data, data_length, 
		   offset, pdb_record_0);

		return offset;
	}

	return -1;
}

/**
 * Free memory allocated to base header.
 * @param *base_h pointer to location that stores base header.
 */
void free_base_header(base_header_t *base_h)
{
	if (base_h->data != NULL)
		free(base_h->data);

	init_base_header(base_h);
}

/**
 * Check if header is of header type.
 * @param base_h base header.
 * @param *header_type header type string.
 * @return uint8_t non-zero if header is of header type.
 */
uint8_t is_header_type(const base_header_t base_h, const char *header_type)
{
	if (!memcmp(base_h.identifier, header_type, sizeof(base_h.identifier)))
		return 1;

	return 0;
}

/**
 * Check if base header identifier is valid.
 * @param base_h base header.
 * @return non-zero if valid.
 */
uint8_t valid_base_identifier(const base_header_t base_h)
{
	return ((base_h.identifier[0] >= 'A') &&
	   (base_h.identifier[1] >= 'A') &&
	   (base_h.identifier[2] >= 'A') &&
	   (base_h.identifier[3] >= 'A'));
}

/**
 * Check if base header length is valid.
 * @param base_h base header.
 * @return non-zero if valid.
 */
uint8_t valid_base_header_length(const base_header_t base_h)
{
	return (base_h.header_length >= 0);
}

/**
 * Check if base header is valid.
 * @param base_h base header.
 * @return non-zero if valid.
 */
uint8_t valid_base_header(const base_header_t base_h)
{
	return (valid_base_identifier(base_h) &&
	   valid_base_header_length(base_h));
}

/**
 * Print base header.
 * @param base_h base header.
 */
void print_base_header(const base_header_t base_h)
{
	printf("Identifier: %s\n", base_h.identifier);
	printf("Header Length: %i\n", base_h.header_length);
	printf("Data: %s\n", base_h.data);
}

