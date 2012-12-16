/**
 * mobipocket.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef MOBIPOCKET_H
#define MOBIPOCKET_H

#include "common.h"
#include "pdb.h"
#include "palmdoc_header.h"
#include "mobi_header.h"
#include "exth_header.h"
#include "lz77.h"

/* Full Name */
#define mobipocket_full_name(mobipocket) mobipocket.full_name
/* Author */
#define mobipocket_author(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_AUTHOR)
#define mobipocket_authors(author, mobipocket, offset_index) \
   exth_record_data(author, mobipocket.exth_header, offset_index, \
      EXTH_RECORD_TYPE_AUTHOR)
/* Publisher */
#define mobipocket_publisher(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_PUBLISHER)
/* Imprint */
#define mobipocket_imprint(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_IMPRINT)
/* Description */
#define mobipocket_description(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_DESCRIPTION)
/* ISBN */
#define mobipocket_isbn(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_ISBN)
/* Subject */
#define mobipocket_subject(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_SUBJECT)
#define mobipocket_subjects(subject, mobipocket, offset_index) \
   exth_record_data(subject, mobipocket.exth_header, offset_index, \
      EXTH_RECORD_TYPE_SUBJECT)
/* Publishing Date */
#define mobipocket_publishing_date(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_PUBLISHINGDATE)
/* Review */
#define mobipocket_review(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_REVIEW)
/* Contributor */
#define mobipocket_contributor(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_CONTRIBUTOR)
/* Rights */
#define mobipocket_rights(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_RIGHTS)
/* ASIN */
#define mobipocket_asin(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_ASIN)
/* Retail Price */
#define mobipocket_retail_price(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_RETAILPRICE)
/* Retail Price Currency */
#define mobipocket_retail_price_currency(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_RETAILPRICECUR)
/* Dictionary Short Name */
#define mobipocket_dictionary_short_name(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_DICTSHORTNAME)
/* Cover Offset */
#define mobipocket_cover_offset(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_COVEROFFSET)
/* Thumbnail Offset */
#define mobipocket_thumb_offset(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_THUMBOFFSET)
/* Creator Software */
#define mobipocket_creator_software(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_CREATORSOFTWARE)
#define mobipocket_creator_software_str(mobipocket) \
   exth_creator_software_str(mobipocket.exth_header)
/* Creator Major */
#define mobipocket_creator_major(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_CREATORMAJOR)
/* Creator Minor */
#define mobipocket_creator_minor(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_CREATORMINOR)
/* Creator Build */
#define mobipocket_creator_build(mobipocket) \
   exth_record_l(mobipocket.exth_header, EXTH_RECORD_TYPE_CREATORBUILD)
/* CDE Type */
#define mobipocket_cde_type(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_CDETYPE)
#define mobipocket_cde_type_str(mobipocket) \
   exth_cde_type_str(mobipocket.exth_header)
/* Updated Title */
#define mobipocket_updated_title(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_UPDATEDTITLE)
/* Language */
#define mobipocket_language(mobipocket) \
   exth_record(mobipocket.exth_header, EXTH_RECORD_TYPE_LANGUAGE)

/* MOBIPocket */
typedef struct {
	pdb_t pdb;
	palmdoc_header_t palmdoc_header;
	mobi_header_t mobi_header;
	exth_header_t exth_header;
	char *full_name;
} mobipocket_t;

void init_mobipocket(mobipocket_t *mobipocket);
uint8_t read_mobipocket(FILE *mobipocket_f, mobipocket_t *mobipocket);
void free_mobipocket(mobipocket_t *mobipocket);

uint8_t is_mobipocket(const mobipocket_t mobipocket);

char* mobipocket_cover(const mobipocket_t mobipocket);
uint32_t mobipocket_cover_length(const mobipocket_t mobipocket);
char* mobipocket_thumb(const mobipocket_t mobipocket);
uint32_t mobipocket_thumb_length(const mobipocket_t mobipocket);

char* mobipocket_image_record(const mobipocket_t mobipocket, uint16_t index);
uint32_t mobipocket_image_record_length(const mobipocket_t mobipocket, 
   uint16_t index);
uint16_t mobipocket_image_record_offset2index(const mobipocket_t mobipocket, 
   uint16_t offset);
uint8_t is_mobipocket_image_record_index(const mobipocket_t mobipocket, 
   uint16_t index);

uint32_t mobipocket_text(char *text, const mobipocket_t mobipocket);
uint32_t mobipocket_text_length(const mobipocket_t mobipocket);
uint32_t mobipocket_text_record(char *text, const mobipocket_t mobipocket, 
   uint16_t index);
uint32_t mobipocket_text_record_length(const mobipocket_t mobipocket, 
   uint16_t index);
uint16_t mobipocket_text_record_offset2index(const mobipocket_t mobipocket, 
   uint16_t offset);
uint8_t is_mobipocket_text_record_index(const mobipocket_t mobipocket, 
   uint16_t index);

#endif /* MOBIPOCKET_H */
