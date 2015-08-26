/*
 *  TFAT, Fat parser and cli
 *  Copyright (C) 2015  Parham Alvani (parham.alvani@gmail.com)
 *  Copyright (C) 2015  Elahe Jalalpour (el.jalalpour@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
*/
#ifndef FAT16_H
#define FAT16_H

#include "FAT.h"

#include <time.h>
#include <stdint.h>
#include <sys/types.h>

struct fat_extBS_16 {
	uint8_t drive_number;
	uint8_t reserved;
	uint8_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t fat_type_label[8];
	uint8_t boot_code[448];
	uint16_t bootable_partition;
} __attribute__((packed));

/* Public struct and vars */

extern struct fat_extBS_16 fat_boot_16;

/* Functions */

/*
 * Buid and initiate
 * fat_boot,
 * fat_table,
 * fat_table_bak,
 * root_dir and data_offset
*/
void init_fat_16(int fd);

/*
 * Free fat_table and
 * fat_table_bak
*/
void free_fat_16(void);

/*
 * Return offset of first sector of given cluster
*/
off_t cluster_to_sector_16(fat_addr_t cluster);

/*
 * Build first cluster address
 * from first_cluster_low and
 * first_cluster_high fields of
 * fat_dir_layout struct
*/
fat_addr_t first_cluster_16(struct fat_dir_layout dir);

/*
 * Find next cluster from fat table
*/
fat_addr_t next_cluster_16(fat_addr_t index);

/*
 * Change cluster numbered index in fat table
 * and fat table backup into new_value
*/
void change_cluster_16(fat_addr_t index, fat_addr_t new_value);

/*
 * Get the value of cluster numbered index
 * int fat table
*/
fat_addr_t get_cluster_16(fat_addr_t index);

fat_addr_t get_cluster_bak_16(fat_addr_t index);


/*
 * Get disk label in null-terminated string
*/
char *get_label_16(void);

/*
 * Set change in backup fat table
 * and write them into disk again.
 * with this function we also write
 * back root directory
 *
 * [EXPERIMENTAL]
*/
void write_fat_16(int fd);

#endif
