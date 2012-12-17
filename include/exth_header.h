/**
 * exth_header.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef EXTH_HEADER_H
#define EXTH_HEADER_H

#include "common.h"
#include "base_header.h"
#include <time.h>

#define HEADER_TYPE_EXTH "EXTH"	/* Header Type: EXTH */
#define is_exth_header(base_h) is_header_type(base_h, HEADER_TYPE_EXTH)

#define EXTH_RECORD_HEADER_LEN 8	/* Length of EXTH record header */

#define EXIT_FAILURE_EXTH 5		/* Failed to read EXTH header */

/* EXTH Record Types */
#define EXTH_RECORD_TYPE_DRMSERVERID 1		/* DRM Server ID */
#define EXTH_RECORD_TYPE_DRMCOMMERCEID 2	/* DRM Commerce ID */
#define EXTH_RECORD_TYPE_DRMEBOOKBASEID 3	/* DRM EBook Base ID */
#define EXTH_RECORD_TYPE_AUTHOR 100		/* Author */
#define EXTH_RECORD_TYPE_PUBLISHER 101		/* Publisher */
#define EXTH_RECORD_TYPE_IMPRINT 102		/* Imprint */
#define EXTH_RECORD_TYPE_DESCRIPTION 103	/* Description */
#define EXTH_RECORD_TYPE_ISBN 104		/* ISBN */
#define EXTH_RECORD_TYPE_SUBJECT 105		/* Subject */
#define EXTH_RECORD_TYPE_PUBLISHINGDATE 106	/* Publishing Date */
#define EXTH_RECORD_TYPE_REVIEW 107		/* Review */
#define EXTH_RECORD_TYPE_CONTRIBUTOR 108	/* Contributor */
#define EXTH_RECORD_TYPE_RIGHTS 109		/* Rights */
#define EXTH_RECORD_TYPE_SUBJECTCODE 110	/* Subject Code */
#define EXTH_RECORD_TYPE_TYPE 111		/* Type */
#define EXTH_RECORD_TYPE_SOURCE 112		/* Source */
#define EXTH_RECORD_TYPE_ASIN 113		/* ASIN */
#define EXTH_RECORD_TYPE_VERSIONNUMBER 114	/* Version Number */
#define EXTH_RECORD_TYPE_SAMPLE 115		/* Sample */
#define EXTH_RECORD_TYPE_STARTREADING 116	/* Start Reading */
#define EXTH_RECORD_TYPE_ADULT 117		/* Adult */
#define EXTH_RECORD_TYPE_RETAILPRICE 118	/* Retail Price */
#define EXTH_RECORD_TYPE_RETAILPRICECUR 119	/* Retail Price Currency */
#define EXTH_RECORD_TYPE_DICTSHORTNAME 200	/* Dictionary Short Name */
#define EXTH_RECORD_TYPE_COVEROFFSET 201	/* Cover Offset */
#define EXTH_RECORD_TYPE_THUMBOFFSET 202	/* Thumbnail Offset */
#define EXTH_RECORD_TYPE_HASFAKECOVER 203	/* Has Fake Cover */
#define EXTH_RECORD_TYPE_CREATORSOFTWARE 204	/* Creator Software */
#define EXTH_RECORD_TYPE_CREATORMAJOR 205	/* Creator Major Version */
#define EXTH_RECORD_TYPE_CREATORMINOR 206	/* Creator Minor Version */
#define EXTH_RECORD_TYPE_CREATORBUILD 207	/* Creator Build Version */
#define EXTH_RECORD_TYPE_WATERMARK 208		/* Watermark */
#define EXTH_RECORD_TYPE_TAMPERPROOF 209	/* Tamper Proof Keys */
#define EXTH_RECORD_TYPE_FONTSIGNATURE 300	/* Font Signature */
#define EXTH_RECORD_TYPE_CLIPPINGLIMIT 401	/* Clipping Limit */
#define EXTH_RECORD_TYPE_PUBLISHERLIMIT 402	/* Publisher Limit */
#define EXTH_RECORD_TYPE_TTSFLAG 404		/* Text to Speech enabled 
						   flag */
#define EXTH_RECORD_TYPE_CDETYPE 501		/* CDE Type */
#define EXTH_RECORD_TYPE_LASTUPDATETIME 502	/* Last updated time */
#define EXTH_RECORD_TYPE_UPDATEDTITLE 503	/* Updated Title */
#define EXTH_RECORD_TYPE_LANGUAGE 524		/* Language */

/* Creator Software IDs */
#define CREATORSOFTWARE_MOBIGEN 1		/* mobigen */
#define CREATORSOFTWARE_MOBIPOCKETCREATOR 2	/* MobiPocket Creator */
#define CREATORSOFTWARE_KINDLEGEN_WINDOWS 200	/* kindlegen (Windows) */
#define CREATORSOFTWARE_KINDLEGEN_LINUX 201	/* kindlegen (Linux) */
#define CREATORSOFTWARE_KINDLEGEN_MAC 202	/* kindlegen (Mac) */
#define CREATORSOFTWARE_MOBIGEN_STR		"mobigen"
#define CREATORSOFTWARE_MOBIPOCKETCREATOR_STR	"MobiPocket Creator"
#define CREATORSOFTWARE_KINDLEGEN_WINDOWS_STR	"kindlegen (Windows)"
#define CREATORSOFTWARE_KINDLEGEN_LINUX_STR	"kindlegen (Linux)"
#define CREATORSOFTWARE_KINDLEGEN_MAC_STR	"kindlegen (Mac)"

/* CDE Types */
#define CDETYPE_PERSONALDOC "PDOC"		/* Personal Doc */
#define CDETYPE_EBOOK "EBOK"			/* EBook */
#define CDETYPE_EBOOKSAMPLE "EBSP"		/* EBook Sample */
#define CDETYPE_PERSONALDOC_STR			"Personal Doc"
#define CDETYPE_EBOOK_STR			"EBook"
#define CDETYPE_EBOOKSAMPLE_STR			"EBook Sample"

/* EXTH Record */
typedef struct {
	uint32_t record_type;
	uint32_t record_length;
	char *record_data;
} exth_record_t;

/* EXTH Header */
typedef struct {
	char identifier[4];
	uint32_t header_length;
	uint32_t record_count;
	exth_record_t *record;
} exth_header_t;

void init_exth_header(exth_header_t *exth_h);
size_t get_exth_header(exth_header_t *exth_h, size_t offset, 
   const char *pdb_record_0);
void convert_base2exth(exth_header_t *exth_h, const base_header_t base_h);
void free_exth_header(exth_header_t *exth_h);

char* exth_record(const exth_header_t exth_h, uint32_t record_type);
uint32_t exth_record_l(const exth_header_t exth_h, uint32_t record_type);
uint32_t exth_record_data(char **record_data, const exth_header_t exth_h, 
   uint32_t offset_index, uint32_t record_type);
uint32_t exth_record_index(const exth_header_t exth_h, 
   uint32_t offset_index, uint32_t record_type);

char* exth_creator_software_str(const exth_header_t exth_h);
uint8_t is_exth_cde_type(const exth_header_t exth_h, const char *cde_type);
char* exth_cde_type_str(const exth_header_t exth_h);
time_t exth_publishing_date_utc(struct tm *time, const exth_header_t exth_h);

void print_exth_header(const exth_header_t exth_h);

#endif /* EXTH_HEADER_H */
