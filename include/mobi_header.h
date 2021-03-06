/**
 * mobi_header.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef MOBI_HEADER_H
#define MOBI_HEADER_H

#include "common.h"
#include "base_header.h"
#include "palmdoc_header.h"

#define HEADER_TYPE_MOBI "MOBI"	/* Header Type: MOBI */
#define is_mobi_header(base_h) is_header_type(base_h, HEADER_TYPE_MOBI)

#define EXIT_FAILURE_MOBI 4	/* Failed to read MOBI header */

/* MOBIPocket File Type */
#define MOBI_TYPE_MOBIPOCKET 2	/* MOBIPocket */
#define MOBI_TYPE_PALMDOC 3	/* PalmDOC */
#define MOBI_TYPE_AUDIO 4	/* Audio */
#define MOBI_TYPE_KINDLEGEN 232	/* MOBIPocket generated by KindleGen */
#define MOBI_TYPE_KF8 248	/* KF8 generated by KindleGen */
#define MOBI_TYPE_NEWS 257	/* News */
#define MOBI_TYPE_NEWSFEED 258	/* News Feed */
#define MOBI_TYPE_MAGAZINE 259	/* News Magazine */
#define MOBI_TYPE_PICS 513	/* PICS */
#define MOBI_TYPE_WORD 514	/* WORD */
#define MOBI_TYPE_XLS 515	/* XLS */
#define MOBI_TYPE_PPT 516	/* PPT */
#define MOBI_TYPE_TEXT 517	/* TEXT */
#define MOBI_TYPE_HTML 518	/* HTML */
#define MOBI_TYPE_MOBIPOCKET_STR	"MOBIPocket"
#define MOBI_TYPE_PALMDOC_STR		"PalmDOC"
#define MOBI_TYPE_AUDIO_STR		"Audio"
#define MOBI_TYPE_KINDLEGEN_STR		"MOBIPocket generated by KindleGen"
#define MOBI_TYPE_KF8_STR		"KF8 generated by KindleGen"
#define MOBI_TYPE_NEWS_STR		"News"
#define MOBI_TYPE_NEWSFEED_STR		"News Feed"
#define MOBI_TYPE_MAGAZINE_STR		"News Magazine"
#define MOBI_TYPE_PICS_STR		"PICS"
#define MOBI_TYPE_WORD_STR		"WORD"
#define MOBI_TYPE_XLS_STR		"XLS"
#define MOBI_TYPE_PPT_STR		"PPT"
#define MOBI_TYPE_TEXT_STR		"TEXT"
#define MOBI_TYPE_HTML_STR		"HTML"

/* Text Encoding */
#define MOBI_ENCODING_WINLATIN1 1252	/* WinLatin1 */
#define MOBI_ENCODING_UTF8 65001	/* UTF-8 */
#define MOBI_ENCODING_WINLATIN1_STR	"iso-8859-1"
#define MOBI_ENCODING_UTF8_STR		"UTF-8"

/* MOBI Header */
typedef struct {
	char identifier[4];
	uint32_t header_length;
	uint32_t mobi_type;
	uint32_t text_encoding;
	uint32_t unique_id;
	uint32_t file_version;
	uint32_t ortographic_index;
	uint32_t inflection_index;
	uint32_t index_names;
	uint32_t index_keys;
	uint32_t extra_index_0;
	uint32_t extra_index_1;
	uint32_t extra_index_2;
	uint32_t extra_index_3;
	uint32_t extra_index_4;
	uint32_t extra_index_5;
	uint32_t first_non_book_index;
	uint32_t full_name_offset;
	uint32_t full_name_length;
	uint32_t locale;
	uint32_t input_language;
	uint32_t output_language;
	uint32_t min_version;
	uint32_t first_image_index;
	uint32_t huffman_record_offset;
	uint32_t huffman_record_count;
	uint32_t huffman_table_offset;
	uint32_t huffman_table_length;
	uint32_t exth_flags;
	uint32_t drm_offset;
	uint32_t drm_count;
	uint32_t drm_size;
	uint32_t drm_flags;
	uint16_t first_content_index;
	uint16_t last_content_index;
	uint32_t fcis_index;
	uint32_t flis_index;
	uint32_t first_compilation_data_section_count;
	uint32_t number_of_compilation_data_sections;
	uint32_t extra_record_data_flags;
	uint32_t indx_offset;
} mobi_header_t;

void init_mobi_header(mobi_header_t *mobi_h);
size_t get_mobi_header(mobi_header_t *mobi_h, const char *pdb_record_0);
void convert_base2mobi(mobi_header_t *mobi_h, const base_header_t base_h);

char* mobi_type_str(const mobi_header_t mobi_h);
char* mobi_text_encoding_str(const mobi_header_t mobi_h);
uint8_t has_exth_header(const mobi_header_t mobi_h);

void print_mobi_header(const mobi_header_t mobi_h);

#endif /* MOBI_HEADER_H */
