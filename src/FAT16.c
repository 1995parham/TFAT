/*
 * In The Name Of God
 * ========================================
 * [] File Name : FAT16.c
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include "FAT.h"
#include "FAT16.h"
#include "common.h"

#include <stdlib.h>
#include <unistd.h>
#include <memory.h>


struct fat_extBS_16 fat_boot_16;
static uint16_t *fat_table;
static uint16_t *fat_table_bak;

static off_t data_offset;

void init_fat_16(int fd)
{
	read(fd, &fat_boot_16, sizeof(fat_boot_16));

	/*
	 * Initiate SECTOR size for
	 * further use
	*/
	SECTOR = fat_boot.bytes_per_sector;

	/*
	 * Seek after reserved sectors
	 * Please note that we read one
	 * of them by now
	*/
	lseek(fd, fat_boot.reserved_sector_count * SECTOR, SEEK_SET);

	fat_table = malloc(SECTOR * fat_boot.table_size_16);
	read(fd, fat_table, SECTOR * fat_boot.table_size_16);

	fat_table_bak = malloc(SECTOR * fat_boot.table_size_16);
	read(fd, fat_table_bak, SECTOR * fat_boot.table_size_16);

	/*
	 * We are going to ignore remain
	 * FAT table !!! ;-)))
	*/
	if (fat_boot.table_count > 2)
		lseek(fd, (fat_boot.table_count - 2) *
		          SECTOR *
		          fat_boot.table_size_16, SEEK_CUR);
	root_dir = malloc(sizeof(struct fat_dir_layout) *
	                  fat_boot.root_entry_count);
	read(fd, root_dir, fat_boot.root_entry_count *
	                   sizeof(struct fat_dir_layout));

	data_offset = lseek(fd, 0, SEEK_CUR);
}

void free_fat_16(void)
{
	free(fat_table);
	free(fat_table_bak);
}

off_t cluster_to_sector_16(fat_addr_t cluster)
{
	return SECTOR * fat_boot.sectors_per_cluster *
	       (cluster - 2) + data_offset;
}

fat_addr_t first_cluster_16(struct fat_dir_layout dir)
{
	return dir.first_cluster_low;
}

fat_addr_t next_cluster_16(fat_addr_t index)
{
	uint16_t nindex = index;

	if ((fat_table[nindex] & 0xF000) == 0xF000)
		return 0x0000;

	return fat_table[nindex];
}

void change_cluster_16(fat_addr_t index, fat_addr_t new_value)
{
	uint16_t nindex = index;
	uint16_t nnew_value = new_value;

	fat_table[nindex] = nnew_value;
	fat_table_bak[nindex] = nnew_value;
}

fat_addr_t get_cluster_16(fat_addr_t index)
{
	uint16_t nindex = index;

	return fat_table[nindex];
}

fat_addr_t get_cluster_bak_16(fat_addr_t index)
{
	uint16_t nindex = index;

	return fat_table_bak[nindex];
}

char *get_label_16(void)
{
	char *ret_label = malloc(12 * sizeof(char));
	int i = 0;

	for (i = 0; i < 11; i++)
		ret_label[i] = fat_boot_16.volume_label[i];
	ret_label[11] = 0;
	return rtrim(ret_label);
}

void write_fat_16(int fd)
{
	memcpy(fat_table_bak, fat_table, SECTOR * fat_boot.table_size_16);
	/*
	 * Write fat_table value into all fat table
	 * available on disk
	*/
	lseek(fd, fat_boot.reserved_sector_count * SECTOR, SEEK_SET);
	int i;

	for (i = 0; i < fat_boot.table_count; i++)
		write(fd, fat_table, fat_boot.table_size_16 * SECTOR);

	write(fd, root_dir, fat_boot.root_entry_count *
	                    sizeof(struct fat_dir_layout));
}
