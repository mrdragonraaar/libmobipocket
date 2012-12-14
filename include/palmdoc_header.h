/**
 * palmdoc_header.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef PALMDOC_HEADER_H
#define PALMDOC_HEADER_H

#include "common.h"

#define PALMDOC_HEADER_LEN 16		/* Length of PalmDOC header */

#define EXIT_FAILURE_PALMDOC 3		/* Failed to read PalmDOC header */

/* Compression */
#define COMPRESSION_NONE 0		/* No compression */
#define COMPRESSION_PALMDOC 2		/* PalmDOC compression */
#define COMPRESSION_HUFF_CDIC 17480	/* HUFF/CDIC compression */

/* Encryption */
#define ENCRYPTION_NONE 0		/* No encryption */
#define ENCRYPTION_MOBI_OLD 1		/* Old Mobipocket encryption */
#define ENCRYPTION_MOBI 2		/* Mobipocket encryption */

/* PalmDOC Header */
typedef struct {
	uint16_t compression;
	uint16_t unused;
	uint32_t text_length;
	uint16_t record_count;
	uint16_t record_size;
	uint16_t encryption_type;
	uint16_t unknown;
} palmdoc_header_t;

void init_palmdoc_header(palmdoc_header_t *palmdoc_h);
size_t get_palmdoc_header(palmdoc_header_t *palmdoc_h, char *pdb_record_0);

uint8_t is_palmdoc_compression(const palmdoc_header_t palmdoc_h, 
   uint16_t compression);
#define is_palmdoc_compression_none(palmdoc_h) \
   is_palmdoc_compression(palmdoc_h, COMPRESSION_NONE)
#define is_palmdoc_compression_palmdoc(palmdoc_h) \
   is_palmdoc_compression(palmdoc_h, COMPRESSION_PALMDOC)
#define is_palmdoc_compression_huff_cdic(palmdoc_h) \
   is_palmdoc_compression(palmdoc_h, COMPRESSION_HUFF_CDIC)

uint8_t is_palmdoc_encryption(const palmdoc_header_t palmdoc_h, 
   uint16_t encryption_type);
#define is_palmdoc_encryption_none(palmdoc_h) \
   is_palmdoc_encryption(palmdoc_h, ENCRYPTION_NONE)
#define is_palmdoc_encryption_mobi_old(palmdoc_h) \
   is_palmdoc_encryption(palmdoc_h, ENCRYPTION_MOBI_OLD)
#define is_palmdoc_encryption_mobi(palmdoc_h) \
   is_palmdoc_encryption(palmdoc_h, ENCRYPTION_MOBI)

void print_palmdoc_header(const palmdoc_header_t palmdoc_h);

#endif /* PALMDOC_HEADER_H */
