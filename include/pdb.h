/**
 * pdb.h
 * 
 * (c)2012 mrdragonraaar.com
 */
#ifndef PDB_H
#define PDB_H

#include "common.h"

#define PDB_HEADER_LEN 32+2+2+(9*4)	/* Length of Palm Database header */
#define PDB_RECORDS_HEADER_LEN 6	/* Length of Palm Database records
					   header */
#define PDB_RECORD_INFO_LEN 8		/* Length of Palm Database record 
					   info */

#define EXIT_FAILURE_PDB_HEADER 1	/* Failed to read Palm Database 
					   header */
#define EXIT_FAILURE_PDB_RECORDS 2	/* Failed to read Palm Database 
					   records */

/* Palm Database Types & Creators */
#define PDB_TYPE_MOBI "BOOK"		/* Type: MOBI */
#define PDB_CREATOR_MOBI "MOBI"		/* Creator: MOBI */
#define PDB_TYPE_PALMDOC "TEXt"		/* Type: PalmDOC */
#define PDB_CREATOR_PALMDOC "REAd"	/* Creator: PalmDOC */

/* Palm Database record */
typedef struct {
	uint32_t record_offset;
	uint8_t record_attributes;
	uint32_t unique_id;
	char *data;
} pdb_record_t;

/* Palm Database records */
typedef struct {
	uint32_t next_record_id;
	uint16_t num_records;
	pdb_record_t *record;
} pdb_records_t;

/* Palm Database header */
typedef struct {
	char name[32];
	uint16_t attributes;
	uint16_t version;
	uint32_t ctime;
	uint32_t mtime;
	uint32_t baktime;
	uint32_t modnum;
	uint32_t appinfo_id;
	uint32_t sortinfo_id;
	char type[4];
	char creator[4];
	uint32_t unique_id_seed;
} pdb_header_t;

/* Palm Database */
typedef struct {
	size_t filesize;
	pdb_header_t header;
	pdb_records_t records;
} pdb_t;

void init_pdb(pdb_t *pdb);
uint8_t read_pdb(FILE *pdb_f, pdb_t *pdb);
void free_pdb(pdb_t *pdb);

size_t get_pdb_record_data_len(const pdb_t pdb, uint16_t index);

uint8_t is_pdb_type(const pdb_t pdb, const char *type, const char *creator);

void print_pdb_header(const pdb_t pdb);
void print_pdb_records_header(const pdb_t pdb);
void print_pdb_record_info(const pdb_t pdb);
void print_pdb_record_data(const pdb_t pdb);

#endif /* PDB_H */
