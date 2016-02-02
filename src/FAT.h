/*
 * In The Name Of God
 * ========================================
 * [] File Name : FAT.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef FAT_H
#define FAT_H

#include <time.h>
#include <stdint.h>
#include <sys/types.h>

struct fat_BS {
	uint8_t			bootjmp[3];
	uint8_t			oem_name[8];
	uint16_t		bytes_per_sector;
	uint8_t			sectors_per_cluster;
	uint16_t		reserved_sector_count;
	uint8_t			table_count;
	uint16_t		root_entry_count;
	uint16_t		total_sectors_16;
	uint8_t			media_type;
	uint16_t		table_size_16;
	uint16_t		sectors_per_track;
	uint16_t		head_side_count;
	uint32_t		hidden_sector_count;
	uint32_t		total_sectors_32;
} __attribute__((packed));

typedef uint32_t fat_addr_t;

struct fat_dir_layout {
	uint8_t		name[8];
	uint8_t		extention[3];
	uint8_t		attr;
	uint8_t		case_information;
	uint8_t		create_second;
	uint16_t	create_time;
	uint16_t	create_date;
	uint16_t	access_date;
	uint16_t	first_cluster_high;
	uint16_t	modification_time;
	uint16_t	modification_date;
	uint16_t	first_cluster_low;
	uint32_t	file_size;
} __attribute__((packed));

/* Public structs and vars */

extern struct fat_BS fat_boot;
extern struct fat_dir_layout *root_dir;

/* *** */

extern fat_addr_t SECTOR;
/*
 * 0: Not initiated
 * 1: FAT16
 * 2: FAT32
 * other: something wrong....
*/
extern int FATN;

/* Functions */

/*
 * Build and initiate
 * fat_boot,
 * fat_table,
 * fat_table_bak,
 * root_dir and data_offset
*/
void init_fat(int fd);

/*
 * Free root_dir
*/
void free_fat(void);

/*
 * Return offset of first sector of given cluster
*/
off_t cluster_to_sector(fat_addr_t cluster);

/*
 * Build first cluster address
 * from first_cluster_low and
 * first_cluster_high fields of
 * fat_dir_layout struct
*/
fat_addr_t first_cluster(struct fat_dir_layout dir);

/*
 * Find next cluster from fat table
*/
fat_addr_t next_cluster(fat_addr_t index);

/*
 * Change cluster numbered index in fat table
 * and fat table backup into new_value
*/
void change_cluster(fat_addr_t index, fat_addr_t new_value);

/*
 * Get the value of cluster numbered index
 * int fat table
*/
fat_addr_t get_cluster(fat_addr_t index);
fat_addr_t get_cluster_bak(fat_addr_t index);

/*
 * FAT table size
*/
fat_addr_t table_size(void);

/*
 * The total number of sectors
*/
fat_addr_t total_sectors(void);

/*
 * Get disk label in null-terminated string
*/
char *get_label(void);

/*
 * Get name in null-terminated string
 * we use case information string to
 * convert name characters into correct
 * case.
 * VFAT specification use following bit:
 * bit 4 : lowercase extention
 * bit 3 : lowercase basename
*/
char *get_name(const uint8_t name[],
		uint8_t case_information);

/*
 * Get extention in null-terminated string
 * we use case information like above
*/
char *get_extension(const uint8_t *const extension,
	uint8_t case_information);

/*
 * Get attributes in string with following
 * format :
 * RHSVDA
 * ------
 * R: Read_Only
 * H: Hidden
 * S: System
 * V: Volume_ID
 * D: Directory
 * A: Archive
*/
char *get_attr(const uint8_t attr);

/*
 * Is directory
*/
int is_directory(const uint8_t attr);

/*
 * Is special file
 * (for more information see [http://www.tavi.co.uk/phobos/fat.html])
 * as OSDev say it contains VOLUME_ID
*/
int is_special(const uint8_t attr);

/*
 * Is deleted
 * name[0] == 0xE5 Ok ?
*/
int is_deleted(const uint8_t name[]);

/*
 * Get create timestamp
*/
struct tm create_time(const uint16_t create_time, const uint16_t create_date);

/*
 * Set change in backup fat table
 * and write them into disk again.
 * with this function we also write
 * back root directory
 *
 * [EXPERIMENTAL]
*/
void write_fat(int fd);

#endif
