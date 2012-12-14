/**
 * pdb.c
 * 
 * (c)2012 mrdragonraaar.com
 */
#include "pdb.h"

/* Default Palm Database header */
const pdb_header_t _pdb_header = { };
/* Default Palm Database records */
const pdb_records_t _pdb_records = { };
/* Default Palm Database record */
const pdb_record_t _pdb_record = { };

size_t read_pdb_header(FILE *pdb_f, pdb_t *pdb);
size_t read_pdb_records(FILE *pdb_f, pdb_t *pdb);
void free_pdb_records(pdb_t *pdb);
size_t read_pdb_records_header(FILE *pdb_f, pdb_t *pdb);
size_t read_pdb_record_info(FILE *pdb_f, pdb_t *pdb);
void init_pdb_record_info(pdb_record_t *pdb_record);
size_t get_pdb_record_info(pdb_record_t *pdb_record, size_t offset, 
   const char *rec_info_buf);
size_t read_pdb_record_data(FILE *pdb_f, pdb_t *pdb);

/**
 * Initialise Palm Database.
 * @param *pdb pointer to location that stores Palm Database.
 */
void init_pdb(pdb_t *pdb)
{
	pdb->filesize = 0;
	pdb->header = _pdb_header;
	pdb->records = _pdb_records;
}

/**
 * Read Palm Database from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return uint8_t exit status. Either EXIT_SUCCESS, EXIT_FAILURE_PDB_HEADER 
 *    or EXIT_FAILURE_PDB_RECORDS.
 */
uint8_t read_pdb(FILE *pdb_f, pdb_t *pdb)
{
	init_pdb(pdb);
	pdb->filesize = get_filesize(pdb_f);

	if (!read_pdb_header(pdb_f, pdb))
		return EXIT_FAILURE_PDB_HEADER;

	if (!read_pdb_records(pdb_f, pdb))
		return EXIT_FAILURE_PDB_RECORDS;

	return EXIT_SUCCESS;
}

/**
 * Free memory allocated to Palm Database.
 * @param *pdb pointer to location that stores Palm Database.
 */
void free_pdb(pdb_t *pdb)
{
	free_pdb_records(pdb);
	init_pdb(pdb);
}

/**
 * Read Palm Database header from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return size_t non-zero on success.
 */
size_t read_pdb_header(FILE *pdb_f, pdb_t *pdb)
{
	size_t res = 0;
	size_t offset = 0;
	char h_buf[PDB_HEADER_LEN];

	if ((res = fread(h_buf, PDB_HEADER_LEN, 1, pdb_f)))
	{
		/* Name */
		offset = get_header_field(pdb->header.name, 
		   sizeof(pdb->header.name), offset, h_buf);
		/* Attributes */
		offset = get_header_field_s(&pdb->header.attributes, offset, 
		   h_buf);
		/* Version */
		offset = get_header_field_s(&pdb->header.version, offset, 
		   h_buf);
		/* Creation Date */
		offset = get_header_field_l(&pdb->header.ctime, offset, h_buf);
		/* Modification Date */
		offset = get_header_field_l(&pdb->header.mtime, offset, h_buf);
		/* Last Backup Date */
		offset = get_header_field_l(&pdb->header.baktime, offset, 
		   h_buf);
		/* Modification Number */
		offset = get_header_field_l(&pdb->header.modnum, offset, h_buf);
		/* Application Info Offset */
		offset = get_header_field_l(&pdb->header.appinfo_id, offset, 
		   h_buf);
		/* Sort Info Offset */
		offset = get_header_field_l(&pdb->header.sortinfo_id, offset, 
		   h_buf);
		/* Type */
		offset = get_header_field(pdb->header.type, 
		   sizeof(pdb->header.type), offset, h_buf);
		/* Creator */
		offset = get_header_field(pdb->header.creator, 
		   sizeof(pdb->header.type), offset, h_buf);
		/* Unique ID Seed */
		get_header_field_l(&pdb->header.unique_id_seed, offset, h_buf);
	}

	return res;
}

/**
 * Read Palm Database records from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return size_t non-zero on success.
 */
size_t read_pdb_records(FILE *pdb_f, pdb_t *pdb)
{
	size_t res = 0;

	if ((res = read_pdb_records_header(pdb_f, pdb)))
		if ((res = read_pdb_record_info(pdb_f, pdb)))
			res = read_pdb_record_data(pdb_f, pdb);

	return res;
}

/**
 * Read Palm Database records header from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return size_t non-zero on success.
 */
size_t read_pdb_records_header(FILE *pdb_f, pdb_t *pdb)
{
	size_t res = 0;
	size_t offset = 0;
	char recs_h_buf[PDB_RECORDS_HEADER_LEN];

	if ((res = fread(recs_h_buf, PDB_RECORDS_HEADER_LEN, 1, pdb_f)))
	{
		/* Next Record ID */
		offset = get_header_field_l(&pdb->records.next_record_id, 
		   offset, recs_h_buf);
		/* Number of Records */
		get_header_field_s(&pdb->records.num_records, offset, 
		   recs_h_buf);
	}

	return res;
}

/**
 * Read Palm Database record info from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return size_t non-zero on success.
 */
size_t read_pdb_record_info(FILE *pdb_f, pdb_t *pdb)
{
	size_t res = 0;

	uint16_t num_records = pdb->records.num_records;
	if (num_records)
	{
		size_t len = PDB_RECORD_INFO_LEN * num_records;
		char rec_info_buf[len];

		if ((res = fread(rec_info_buf, len, 1, pdb_f)))
		{
			pdb->records.record = malloc(num_records * 
			   sizeof(pdb_record_t));

			size_t offset = 0;
			uint16_t i;
			for (i = 0; i < num_records; i++)
			{
				offset = get_pdb_record_info(
				   &pdb->records.record[i], offset, 
				   rec_info_buf);
			}
		}
	}

	return res;
}

/**
 * Initialise Palm Database record info.
 * @param *pdb_record pointer to location that stores Palm Database record.
 */
void init_pdb_record_info(pdb_record_t *pdb_record)
{
	*pdb_record = _pdb_record;
}

/**
 * Get Palm Database record info from record info buffer.
 * @param *pdb_record pointer to location that stores Palm Database record.
 * @param offset offset in record info buffer.
 * @param *rec_info_buf pointer to record info buffer.
 * @return size_t new offset in record info buffer.
 */
size_t get_pdb_record_info(pdb_record_t *pdb_record, size_t offset, 
   const char *rec_info_buf)
{
	char info[PDB_RECORD_INFO_LEN];
	size_t info_offset = 0;

	offset = get_header_field(info, sizeof(info), offset, rec_info_buf);

	init_pdb_record_info(pdb_record);

	/* Record Offset */
	info_offset = get_header_field_l(&pdb_record->record_offset, 
	   info_offset, info);
	/* Record Attributes */
	info_offset = get_header_field_c(&pdb_record->record_attributes, 
	   info_offset, info);
	/* Unique ID */
	get_header_field_uint24(&pdb_record->unique_id, info_offset, info);

	return offset;
}

/**
 * Read Palm Database record data from open file stream.
 * @param *pdb_f open file stream of PDB file.
 * @param *pdb pointer to location that stores Palm Database.
 * @return size_t non-zero on success.
 */
size_t read_pdb_record_data(FILE *pdb_f, pdb_t *pdb)
{
	size_t res = 0;

	if (pdb->records.record != NULL)
	{
		uint16_t num_records = pdb->records.num_records;
		uint16_t i;
		for (i = 0; i < num_records; i++)
		{
			uint32_t offset = pdb->records.record[i].record_offset;

			if (ftell(pdb_f) != offset)
				fseek(pdb_f, offset, SEEK_SET);

			size_t len;
			if ((len = get_pdb_record_data_len(*pdb, i)))
			{
				pdb->records.record[i].data = 
				   malloc(len * sizeof(char));

				if (!(res = fread(pdb->records.record[i].data,
				   len, 1, pdb_f)))
					return res;
			}

		}
	}

	return res;
}

/**
 * Get the length of Palm Database record data.
 * @param pdb Palm Database.
 * @param index index of Palm Database record.
 * @return size_t length of Palm Database record data.
 */
size_t get_pdb_record_data_len(const pdb_t pdb, uint16_t index)
{
	if (pdb.records.num_records && (pdb.records.record != NULL))
	{
		if (index < pdb.records.num_records - 1)
			return pdb.records.record[index+1].record_offset - 
			   pdb.records.record[index].record_offset;

		return pdb.filesize - pdb.records.record[index].record_offset;
	}

	return 0;
}

/**
 * Free memory allocated to Palm Database records.
 * @param *pdb pointer to location that stores Palm Database.
 */
void free_pdb_records(pdb_t *pdb)
{
	if (pdb->records.record != NULL)
	{
		uint16_t num_records = pdb->records.num_records;
		uint16_t i;
		for (i = 0; i < num_records; i++)
		{
			if (pdb->records.record[i].data != NULL)
				free(pdb->records.record[i].data);
		}

		free(pdb->records.record);
		pdb->records.record = NULL;
	}
}

/**
 * Check type and creator of Palm Database.
 * @param pdb Palm Database.
 * @param *type type string.
 * @param *creator creator string.
 * @return uint8_t non-zero if PDB is of type and creator.
 */
uint8_t is_pdb_type(const pdb_t pdb, const char *type, const char *creator)
{
	if (!memcmp(pdb.header.type, type, sizeof(pdb.header.type)) &&
	   !memcmp(pdb.header.creator, creator, sizeof(pdb.header.creator)))
		return 1;

	return 0;
}

/**
 * Print Palm Database header.
 * @param pdb Palm Database.
 */
void print_pdb_header(const pdb_t pdb)
{
	printf("Name: %s\n", pdb.header.name);
	printf("Attributes: %i\n", pdb.header.attributes);
	printf("Version: %i\n", pdb.header.version);
	printf("CTime: %i\n", pdb.header.ctime);
	printf("MTime: %i\n", pdb.header.mtime);
	printf("BakTime: %i\n", pdb.header.baktime);
	printf("ModNum: %i\n", pdb.header.modnum);
	printf("AppInfo ID: %i\n", pdb.header.appinfo_id);
	printf("SortInfo ID: %i\n", pdb.header.sortinfo_id);
	printf("Type: %c%c%c%c\n", pdb.header.type[0], pdb.header.type[1], 
	   pdb.header.type[2], pdb.header.type[3]);
	printf("Creator: %c%c%c%c\n", pdb.header.creator[0], 
	   pdb.header.creator[1], pdb.header.creator[2], pdb.header.creator[3]);
	printf("Unique ID Seed: %i\n", pdb.header.unique_id_seed);
}

/**
 * Print Palm Database records header.
 * @param pdb Palm Database.
 */
void print_pdb_records_header(const pdb_t pdb)
{
	printf("Next Record ID: %i\n", pdb.records.next_record_id);
	printf("Num of Records: %i\n", pdb.records.num_records);
}

/**
 * Print Palm Database record info.
 * @param pdb Palm Database.
 */
void print_pdb_record_info(const pdb_t pdb)
{
	uint16_t num_records = pdb.records.num_records;
	uint16_t i;
	for (i = 0; i < num_records; i++)
	{
		printf("Record Offset: %i\n", 
		   pdb.records.record[i].record_offset);
		printf("Record Attributes: %i\n", 
		   pdb.records.record[i].record_attributes);
		printf("Unique ID: %i\n", pdb.records.record[i].unique_id);
	}
}

/**
 * Print Palm Database record data.
 * @param pdb Palm Database.
 */
void print_pdb_record_data(const pdb_t pdb)
{
	uint16_t num_records = pdb.records.num_records;
	uint16_t i;
	for (i = 0; i < num_records; i++)
	{
		printf("Record %i\n", i);
		printf("%s\n", pdb.records.record[i].data);
	}
}

