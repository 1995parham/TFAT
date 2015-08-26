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
#include "FAT.h"
#include "FAT32.h"
#include "common.h"

#include <stdlib.h>
#include <unistd.h>
#include <memory.h>


struct fat_extBS_32 fat_boot_32;
struct fat_info_32 fat_info;

static uint32_t *fat_table;
static uint32_t *fat_table_bak;
static off_t data_offset;

void init_fat_32(int fd)
{
	read(fd, &fat_boot_32, sizeof(fat_boot_32));

	/* Seek to FS information sector to read
	 * fat_info
	*/
	lseek(fd, fat_boot_32.fat_info * SECTOR, SEEK_SET);
	read(fd, &fat_info, sizeof(fat_info));

	/*
	 * Seek after reserved sectors
	 * Please note that we read one
	 * of them by now
	*/
	lseek(fd, fat_boot.reserved_sector_count * SECTOR, SEEK_SET);

	fat_table = malloc(SECTOR * fat_boot_32.table_size_32);
	read(fd, fat_table, SECTOR * fat_boot_32.table_size_32);

	fat_table_bak = malloc(SECTOR * fat_boot_32.table_size_32);
	read(fd, fat_table_bak, SECTOR * fat_boot_32.table_size_32);

	/*
	 * We are going to ignore remain
	 * FAT table !!! ;-)))
	*/
	if (fat_boot.table_count > 2)
		lseek(fd, (fat_boot.table_count - 2) *
		          SECTOR *
		          fat_boot_32.table_size_32, SEEK_CUR);

	data_offset = lseek(fd, 0, SEEK_CUR);

	/*
	 * We are going to read root_dir
	*/

	int index = 0;
	fat_addr_t cluster = fat_boot_32.root_cluster;

	root_dir = malloc(SECTOR * fat_boot.sectors_per_cluster);
	while (cluster) {
		lseek(fd, (cluster - 2) * fat_boot.sectors_per_cluster *
		          SECTOR, SEEK_CUR);
		read(fd, root_dir + index, SECTOR *
		                           fat_boot.sectors_per_cluster);
		fat_boot.root_entry_count +=
			(SECTOR * fat_boot.sectors_per_cluster) /
			sizeof(struct fat_dir_layout);
		index += SECTOR * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}

}

void free_fat_32(void)
{
	free(fat_table);
	free(fat_table_bak);
}

off_t cluster_to_sector_32(fat_addr_t cluster)
{
	return SECTOR * fat_boot.sectors_per_cluster *
	       (cluster - 2) + data_offset;
}

fat_addr_t first_cluster_32(struct fat_dir_layout dir)
{
	return dir.first_cluster_high << 16 | dir.first_cluster_low;
}

fat_addr_t next_cluster_32(fat_addr_t index)
{
	if ((fat_table[index] & 0x0F000000) == 0x0F000000)
		return 0x00000000;

	/* FAT 32 uses 28 bits to address the clusters on the disk. */
	return fat_table[index] & 0x0FFFFFFF;
}

void change_cluster_32(fat_addr_t index, fat_addr_t new_value)
{
	fat_table[index] = new_value;
	fat_table_bak[index] = new_value;
}

fat_addr_t get_cluster_32(fat_addr_t index)
{
	return fat_table[index];
}

fat_addr_t get_cluster_bak_32(fat_addr_t index)
{
	return fat_table_bak[index];
}

char *get_label_32(void)
{
	char *ret_label = malloc(12 * sizeof(char));
	int i = 0;

	for (i = 0; i < 11; i++)
		ret_label[i] = fat_boot_32.volume_label[i];
	ret_label[11] = 0;
	return rtrim(ret_label);
}

void write_fat_32(int fd)
{
	memcpy(fat_table_bak, fat_table, SECTOR * fat_boot_32.table_size_32);

	/*
	 * Write fat_table value into all fat table
	 * available on disk
	*/
	lseek(fd, fat_boot.reserved_sector_count * SECTOR, SEEK_SET);

	int i;

	for (i = 0; i < fat_boot.table_count; i++)
		write(fd, fat_table, fat_boot_32.table_size_32 * SECTOR);

	/*
	 * Write root_dir back on the disk
	*/

	int index = 0;
	fat_addr_t cluster = fat_boot_32.root_cluster;

	while (cluster) {
		lseek(fd, (cluster - 2) * fat_boot.sectors_per_cluster *
		          SECTOR, SEEK_CUR);
		write(fd, root_dir + index, SECTOR *
		                            fat_boot.sectors_per_cluster);
		fat_boot.root_entry_count +=
			(SECTOR * fat_boot.sectors_per_cluster) /
			sizeof(struct fat_dir_layout);
		index += SECTOR * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}
}
