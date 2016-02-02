/*
 * In The Name Of God
 * ========================================
 * [] File Name : FAT32.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef FAT32_H
#define FAT32_H

#include "FAT.h"

#include <time.h>
#include <stdint.h>
#include <sys/types.h>

/*
 * @signature_0: FS information sector signature (0x52 0x52 0x61 0x41 = "RRaA")
 *
 * @reserved_0: Reserved (byte values should be set to 0x00 during format,
 * but not be relied upon and never changed later on)
 *
 * @signature_1: FS information sector signature (0x72 0x72 0x41 0x61 = "rrAa")
 *
 * @free_clusters: Last known number of free data clusters on the volume,
 * or 0xFFFFFFFF if unknown.
 *
 * @recently_alloc_clusters: Number of the most recently known to be
 * allocated data cluster. Should be set to 0xFFFFFFFF during format
 * and updated by the operating system later on.
 *
 * @reserved_1: Reserved (byte values should be set to 0x00 during format,
 * but not be relied upon and never changed later on)
 *
 * @signature_2: FS information sector signature (0x00 0x00 0x55 0xAA)
 * (All four bytes should match before the contents of this sector should
 * be assumed to be in valid format.)
*/
struct fat_info_32 {
	uint32_t signature_0;
	uint8_t reserved_0[480];
	/* Why ??? */
	uint32_t signature_1;
	uint32_t free_clusters;
	uint32_t recently_alloc_clusters;
	uint8_t reserved_1[12];
	uint32_t signature_2;
} __attribute__((packed));

struct fat_extBS_32 {
	uint32_t table_size_32;
	uint16_t extended_flags;
	uint16_t fat_version;
	uint32_t root_cluster;
	uint16_t fat_info;
	uint16_t backup_BS_sector;
	uint8_t reserved_0[12];
	uint8_t drive_number;
	uint8_t reserved_1;
	uint8_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t fat_type_label[8];
	uint8_t boot_code[420];
	uint16_t bootable_partition;
} __attribute__((packed));

/* Public structs and vars */

extern struct fat_extBS_32 fat_boot_32;
extern struct fat_info_32 fat_info;

/* Functions */

/*
 * Buid and initiate
 * fat_boot,
 * fat_table,
 * fat_table_bak,
 * root_dir and data_offset
*/
void init_fat_32(int fd);

/*
 * Free fat_table and
 * fat_table_bak
*/
void free_fat_32(void);


/*
 * Return offset of first sector of given cluster
*/
off_t cluster_to_sector_32(fat_addr_t cluster);

/*
 * Build first cluster address
 * from first_cluster_low and
 * first_cluster_high fields of
 * fat_dir_layout struct
*/
fat_addr_t first_cluster_32(struct fat_dir_layout dir);

/*
 * Find next cluster from fat table
*/
fat_addr_t next_cluster_32(fat_addr_t index);

/*
 * Change cluster numbered index in fat table
 * and fat table backup into new_value
*/
void change_cluster_32(fat_addr_t index, fat_addr_t new_value);

/*
 * Get the value of cluster numbered index
 * int fat table
*/
fat_addr_t get_cluster_32(fat_addr_t index);

fat_addr_t get_cluster_bak_32(fat_addr_t index);

/*
 * Get disk label in null-terminated string
*/
char *get_label_32(void);

/*
 * Set change in backup fat table
 * and write them into disk again.
 * with this function we also write
 * back root directory
 *
 * [EXPERIMENTAL]
*/
void write_fat_32(int fd);

#endif
