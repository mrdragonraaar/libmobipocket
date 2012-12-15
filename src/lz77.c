/**
 * lz77.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "lz77.h"

/**
 * Decode LZ77 compressed buffer.
 * @param *buf_plain Decoded buffer.
 * @param *buf_compressed LZ77 compressed buffer.
 * @param compressed_len length of LZ77 compressed buffer.
 * @return number of bytes decoded.
 */
uint16_t lz77_decode(char *buf_plain, const char *buf_compressed, 
   uint16_t compressed_len)
{
	uint8_t *bufout = (uint8_t *)buf_plain;
	const uint8_t *bufin = (const uint8_t *)buf_compressed;

	uint16_t i = 0;
	while (i < compressed_len)
	{
		uint16_t c = *(bufin++) & 0x00FF;

		/* byte pair (space + char) */
		if (c >= 0xC0)
		{
			*(bufout++) = ' ';
			*(bufout++) = c ^ 0x80;
		}
		/* length, distance pair */
		else if (c >= 0x80)
		{
			c = (c << 8) | (*(bufin++) & 0x00FF);
			int len = 3 + (c & 0x0007);
			int dist = (c >> 3) & 0x07FF;
			uint8_t *buf = (uint8_t *)bufout - dist;
			while (len-- > 0)
				*(bufout++) = *(buf++);
		}
		/* literal */
		else if (c >= 0x09)
			*(bufout++) = c;
		/* c literals */
		else if (c >= 0x01)
			while (c-- > 0)
				*(bufout++) = *(bufin++);
		/* literal */
		else
			*(bufout++) = c;

		i = bufin - (const uint8_t *)buf_compressed;
	}

	return bufout - (const uint8_t *)buf_plain;
}

/**
 * Get length of decoded LZ77 compressed buffer.
 * @param *buf_compressed LZ77 compressed buffer.
 * @param compressed_len length of LZ77 compressed buffer.
 * @return length of decoded buffer.
 */
uint16_t lz77_decode_length(const char *buf_compressed, uint16_t compressed_len)
{
	uint16_t len = 0;
	const uint8_t *bufin = (const uint8_t *)buf_compressed;

	uint16_t i = 0;
	while (i < compressed_len)
	{
		uint16_t c = *(bufin++) & 0x00FF;

		/* byte pair (space + char) */
		if (c >= 0xC0)
			len += 2;
		/* length, distance pair */
		else if (c >= 0x80)
		{
			c = (c << 8) | (*(bufin++) & 0x00FF);
			len += 3 + (c & 0x0007);
		}
		/* literal */
		else if (c >= 0x09)
			len++;
		/* c literals */
		else if (c >= 0x01)
		{
			len += c;
			bufin += c;
		}
		/* literal */
		else
			len++;

		i = bufin - (const uint8_t *)buf_compressed;
	}

	return len;
}

