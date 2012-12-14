/**
 * base_header.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef BASE_HEADER_H
#define BASE_HEADER_H

#include "common.h"

#define BASE_HEADER_LEN 8;	/* Length of a header entries' header.
				   Size of identifier and header length 
				   fields. */

/* Base Header */
typedef struct {
	char identifier[4];
	uint32_t header_length;
	char *data;
} base_header_t;

void init_base_header(base_header_t *base_h);
size_t get_base_header(base_header_t *base_h, size_t offset, 
   const char *pdb_record_0);
void free_base_header(base_header_t *base_h);

uint8_t is_header_type(const base_header_t base_h, const char *header_type);

void print_base_header(const base_header_t base_h);

#endif /* BASE_HEADER_H */
