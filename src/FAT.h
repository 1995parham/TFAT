// In The Name Of God
// ========================================
// * File Name : FAT.h
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Fri 19 Dec 2014 05:48:57 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#ifndef FAT_H
#define FAT_H

#include <time.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct{
	uint8_t 	bios_drive_num;
	uint8_t 	reserved1;
	uint8_t 	boot_signature;
	uint32_t	volume_id;
	uint8_t		volume_label[11];
	uint8_t		fat_type_label[8];
	uint8_t		boot_code[448];
	uint16_t	bootable_partition;
}__attribute__((packed)) fat_extBS_16_t;

typedef struct{
	uint8_t 	bootjmp[3];
	uint8_t 	oem_name[8];
	uint16_t	bytes_per_sector;
	uint8_t		sectors_per_cluster;
	uint16_t	reserved_sector_count;
	uint8_t		table_count;
	uint16_t	root_entry_count;
	uint16_t	total_sectors_16;
	uint8_t		media_type;
	uint16_t	table_size_16;
	uint16_t	sectors_per_track;
	uint16_t	head_side_count;
	uint32_t	hidden_sector_count;
	uint32_t	total_sectors_32;
	fat_extBS_16_t	extBS;															 
}__attribute__((packed)) fat_BS_t;

typedef uint16_t fat_addr_t;

typedef struct{
	uint8_t		name[8];
	uint8_t		extention[3];
	uint8_t		attr;
	uint8_t		reserved;
	uint8_t		create_second;
	uint16_t	create_time;
	uint16_t	create_date;
	uint16_t	access_date;
	uint16_t	zero;		// Why ???
	uint16_t	modification_time;
	uint16_t	modification_date;
	uint16_t	first_cluster;
	uint32_t	file_size;
}__attribute__((packed)) fat_dir_layout_t;

typedef struct{
	uint8_t 	order;
	uint16_t 	first_name[5];
	uint8_t 	attr; 		// Always equals 0x0F
	uint8_t 	type;
	uint8_t 	checksum;
	uint16_t 	last_name[6];
	uint16_t 	zero; 		// Why ???
	uint16_t 	final_name[2];
}__attribute__((packed)) fat_dir_long_name_t;

// Public structs and vars

extern fat_BS_t fat_boot;
extern fat_addr_t *fat_table;
extern fat_addr_t *fat_table_bak;
extern fat_dir_layout_t *root_dir;
extern off_t data_offset;

// Functions

// Buid and initiate fat, fat_table, fat_table_bak, root_dir and data_offset
void init_fat(int fd);
// Find next cluster from fat table
fat_addr_t next_cluster(fat_addr_t index);
// The first in the File Allocation Table
fat_addr_t first_fat_sector();
// The size of the root directory
fat_addr_t root_dir_sectors();
// The first data sector (that is, the first sector in which directories and files may be stored)
fat_addr_t first_data_sector();
// The total number of data sectors
fat_addr_t data_sectors();
// The total number of clusters
fat_addr_t total_clusters();
// Get name in null-terminated string
char* get_name(uint8_t name[]);
// Get extention in null-terminated string
char* get_extention(uint8_t extention[]);
// Is directory
int is_directory(uint8_t attr);
// Is special file (for more information see [http://www.tavi.co.uk/phobos/fat.html])
// as OSDev say it contains VOLUME_ID
int is_special(uint8_t attr);
// Get create timestamp
struct tm create_time(uint16_t create_time, uint16_t create_date);

#endif
