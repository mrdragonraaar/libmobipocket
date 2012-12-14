/**
 * common.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "common.h"

/**
 * Get file size of open file stream.
 * @param *fd open file stream.
 * @return size_t file size.
 */
size_t get_filesize(FILE *fd)
{
	size_t pos = ftell(fd);

	fseek(fd, 0, SEEK_END);
	size_t len = ftell(fd);

	fseek(fd, pos, SEEK_SET);
	return len;
}

/**
 * Get field of size starting at offset from buffer.
 * @param *field pointer to location that stores field.
 * @param size size of field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field(void *field, size_t size, size_t offset, 
   const char *field_buf)
{
	if (field_buf != NULL)
		memcpy(field, field_buf+offset, size);
	offset += size;

	return offset;
}

/**
 * Get char* field of size starting at offset from buffer.
 * Allocates memory for string and adds terminating NULL.
 * @param **field pointer to location that stores char* field.
 * @param size size of field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field_str(char **field, size_t size, size_t offset, 
   const char *field_buf)
{
	*field = malloc((size + 1) * sizeof(char));
	offset = get_header_field(*field, size, offset, field_buf);
	(*field)[size] = '\0';

	return offset;
}

/**
 * Get uint8_t field starting at offset from buffer.
 * @param *field pointer to location that stores uint8_t field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field_c(uint8_t *field, size_t offset, const char *field_buf)
{
	return get_header_field(field, sizeof(uint8_t), offset, field_buf);
}

/**
 * Get uint16_t field starting at offset from buffer.
 * @param *field pointer to location that stores uint16_t field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field_s(uint16_t *field, size_t offset, const char *field_buf)
{
	offset = get_header_field(field, sizeof(uint16_t), offset, field_buf);
	*field = ntohs(*field);

	return offset;
}

/**
 * Get uint24_t field starting at offset from buffer. Stored in uint32_t.
 * @param *field pointer to location that stores uint24_t field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field_uint24(uint32_t *field, size_t offset, 
   const char *field_buf)
{
	uint8_t field_arr[3];

	offset = get_header_field(field_arr, sizeof(field_arr), offset, 
	   field_buf);
	*field = (field_arr[0] << 16) | (field_arr[1] << 8) | field_arr[2];

	return offset;
}

/**
 * Get uint32_t field starting at offset from buffer.
 * @param *field pointer to location that stores uint32_t field.
 * @param offset offset in buffer.
 * @param *field_buf pointer to buffer.
 * @return size_t new offset in buffer.
 */
size_t get_header_field_l(uint32_t *field, size_t offset, const char *field_buf)
{
	offset = get_header_field(field, sizeof(uint32_t), offset, field_buf);
	*field = ntohl(*field);

	return offset;
}

