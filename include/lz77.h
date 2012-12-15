/**
 * lz77.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef LZ77_H
#define LZ77_H

#include <stdint.h>

uint16_t lz77_decode(char *buf_plain, const char *buf_compressed, 
   uint16_t compressed_len);
uint16_t lz77_decode_length(const char *buf_compressed, 
   uint16_t compressed_len);

#endif /* LZ77_H */
