/**
 * common.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

size_t get_filesize(FILE *fd);

size_t get_header_field(void *field, size_t size, size_t offset, 
   const char *field_buf);
size_t get_header_field_str(char **field, size_t size, size_t offset, 
   const char *field_buf);
size_t get_header_field_c(uint8_t *field, size_t offset, const char *field_buf);
size_t get_header_field_s(uint16_t *field, size_t offset, 
   const char *field_buf);
size_t get_header_field_uint24(uint32_t *field, size_t offset, 
   const char *field_buf);
size_t get_header_field_l(uint32_t *field, size_t offset, 
   const char *field_buf);

#endif /* COMMON_H */
