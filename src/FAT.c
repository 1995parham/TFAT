/*
 * In The Name Of God
 * ========================================
 * [] File Name : FAT.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Thu 01 Jan 2015 07:51:31 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include "FAT.h"
#include "common.h"
#include "FAT16.h"
#include "FAT32.h"

#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>


struct fat_BS fat_boot;
struct fat_info_32 fat_info;
fat_addr_t *fat_table;
fat_addr_t *fat_table_bak;
struct fat_dir_layout *root_dir;

fat_addr_t SECTOR;
int FATN;

void init_fat(int fd)
{
	read(fd, &fat_boot, sizeof(fat_boot));

	/*
	 * Initiate SECTOR size for
	 * further use
	*/
	SECTOR = fat_boot.bytes_per_sector;

	/*
	 * Initiate FATN value for
	 * further use
	*/
	if (fat_boot.total_sectors_16 == 0)
		FATN = 2;
	else
		FATN = 1;

	if (FATN == 1)
		init_fat_16(fd);
	else
		init_fat_32(fd);
}

void free_fat(void)
{
	if (FATN == 1)
		free_fat_16();
	else
		free_fat_32();
	free(root_dir);
}

off_t cluster_to_sector(fat_addr_t cluster)
{
	if (FATN == 1)
		return cluster_to_sector_16(cluster);
	else
		return cluster_to_sector_32(cluster);
}

fat_addr_t first_cluster(struct fat_dir_layout dir)
{
	if (FATN == 1)
		return first_cluster_16(dir);
	else
		return first_cluster_32(dir);
}

fat_addr_t next_cluster(fat_addr_t index)
{
	if (FATN == 1)
		return next_cluster_16(index);
	else
		return next_cluster_32(index);
}

void change_cluster(fat_addr_t index, fat_addr_t new_value)
{
	if (FATN == 1)
		return change_cluster_16(index, new_value);
	else
		return change_cluster_32(index, new_value);
}

fat_addr_t get_cluster(fat_addr_t index)
{
	if (FATN == 1)
		return get_cluster_16(index);
	else
		return get_cluster_32(index);
}

fat_addr_t get_cluster_bak(fat_addr_t index)
{
	if (FATN == 1)
		return get_cluster_bak_16(index);
	else
		return get_cluster_bak_32(index);
}

fat_addr_t table_size(void)
{
	if (FATN == 1)
		return fat_boot.table_size_16 * (SECTOR / 2);
	else
		return fat_boot_32.table_size_32 * (SECTOR / 4);
}

fat_addr_t total_sectors(void)
{
	if (FATN == 1)
		return fat_boot.total_sectors_16;
	else
		return fat_boot.total_sectors_32;
}

int is_directory(const uint8_t attr)
{
	return attr & 0x10;
}

int is_special(const uint8_t attr)
{
	return attr & 0x08;
}

int is_deleted(const uint8_t name[])
{
	return name[0] == 0xE5;
}

char *get_label(void)
{
	if (FATN == 1)
		return get_label_16();
	else
		return get_label_32();
}

/*
 * name[0] =
 * 0x00: Filenaem never used
 * 0xE5: The filename has been used, but the file has been deleted
 * 0x05: The first character of filename is actually 0xe5
*/
char *get_name(const uint8_t name[],
		uint8_t case_information)
{
	if (name[0] == 0x00)
		return NULL;

	char *ret_name = malloc(9 * sizeof(char));
	int lower = case_information & 0x8;

	if (name[0] == 0xE5)
		ret_name[0] = '?';
	else if (name[0] == 0x05)
		ret_name[0] = 0xE5;
	else
		ret_name[0] = (lower) ? tolower(name[0]) : name[0];
	int i;

	for (i = 1; i < 8; i++)
		ret_name[i] = (lower) ? tolower(name[i]) : name[i];
	ret_name[8] = 0;
	return rtrim(ret_name);
}

char *get_extention(const uint8_t extention[],
		uint8_t case_information)
{
	char *ret_extention = malloc(4 * sizeof(char));
	int lower = case_information & 0x10;
	int i;

	for (i = 0; i < 3; i++)
		ret_extention[i] = (lower) ?
			tolower(extention[i]) : extention[i];
	ret_extention[3] = 0;
	return rtrim(ret_extention);
}

char *get_attr(const uint8_t attr)
{
	char *ret_attr = malloc(7 * sizeof(char));

	if (attr & 0x01)
		ret_attr[0] = 'R';
	else
		ret_attr[0] = '-';
	if (attr & 0x02)
		ret_attr[1] = 'H';
	else
		ret_attr[1] = '-';
	if (attr & 0x04)
		ret_attr[2] = 'S';
	else
		ret_attr[2] = '-';
	if (attr & 0x08)
		ret_attr[3] = 'V';
	else
		ret_attr[3] = '-';
	if (attr & 0x10)
		ret_attr[4] = 'D';
	else
		ret_attr[4] = '-';
	if (attr & 0x20)
		ret_attr[5] = 'A';
	else
		ret_attr[5] = '-';
	ret_attr[6] = 0;
	return ret_attr;

}

struct tm create_time(const uint16_t create_time, const uint16_t create_date)
{
	struct tm file_tm;

	/*
	 * 0b1111111000000000 = 0xFE00
	*/
	file_tm.tm_year = ((create_date & 0xFE00) >> 9) + 80;

	/*
	 * 0b0000000111100000 = 0x1E0
	*/
	file_tm.tm_mon  = ((create_date & 0x1E0) >> 5) - 1;

	/*
	 * 0b0000000000011111 = 0x1F
	*/
	file_tm.tm_mday = (create_date & 0x1F);

	/*
	 * 0b1111100000000000 = 0xF800
	*/
	file_tm.tm_hour = (create_time & 0xF800) >> 11;

	/*
	 * 0b0000011111100000 = 0x7E0
	*/
	file_tm.tm_min  = (create_time & 0x7E0) >> 5;

	/*
	 * 0b0000000000011111 = 0x1F
	*/
	file_tm.tm_sec  = (create_time & 0x1F) * 2;
	return file_tm;
}

void write_fat(int fd)
{
	if (FATN == 1)
		write_fat_16(fd);
	else
		write_fat_32(fd);
}
