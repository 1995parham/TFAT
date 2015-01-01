/*
 * In The Name Of God
 * ========================================
 * [] File Name : FAT.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Thu 01 Jan 2015 05:42:00 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include "FAT.h"
#include "common.h"

#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>


struct fat_BS fat_boot;
fat_addr_t *fat_table;
fat_addr_t *fat_table_bak;
struct fat_dir_layout *root_dir;
off_t data_offset;
fat_addr_t SECTOR;

void init_fat(int fd)
{
	read(fd, &fat_boot, 512);

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

	fat_table = malloc(sizeof(fat_addr_t) *
			(SECTOR  / 2) * fat_boot.table_size_16);
	read(fd, fat_table, SECTOR * fat_boot.table_size_16);

	fat_table_bak = malloc(sizeof(fat_addr_t) *
			(SECTOR / 2) * fat_boot.table_size_16);
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

void free_fat(void)
{
	free(fat_table);
	free(fat_table_bak);
	free(root_dir);
}

fat_addr_t next_cluster(fat_addr_t index)
{
	if ((fat_table[index] & 0xF000) == 0xF000)
		return 0x0000;
	return fat_table[index];
}

void change_cluster(fat_addr_t index, fat_addr_t new_value)
{
	fat_table[index] = new_value;
	fat_table_bak[index] = new_value;
}

fat_addr_t root_dir_sectors(void)
{
	return ((fat_boot.root_entry_count * 32) +
			(fat_boot.bytes_per_sector - 1)) /
		fat_boot.bytes_per_sector;

}

fat_addr_t first_fat_sector(void)
{
	return  fat_boot.reserved_sector_count;
}

fat_addr_t first_data_sector(void)
{
	return fat_boot.reserved_sector_count +
		(fat_boot.table_count *
		 fat_boot.table_size_16);
}

fat_addr_t data_sectors(void)
{
	return fat_boot.total_sectors_16 -
		(fat_boot.reserved_sector_count +
		 (fat_boot.table_count *
		  fat_boot.table_size_16) +
		 root_dir_sectors());
}

fat_addr_t total_clusters(void)
{
	return (data_sectors() /
			fat_boot.sectors_per_cluster);
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
	char *ret_label = malloc(12 * sizeof(char));
	int i = 0;

	for (i = 0; i < 11; i++)
		ret_label[i] = fat_boot.extBS.volume_label[i];
	ret_label[11] = 0;
	return rtrim(ret_label);
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

	/*
	 * TODO please fix following condition
	*/
	if (name[0] == 0xE5)
		;
	char *ret_name = malloc(9 * sizeof(char));
	int lower = case_information & 0x8;

	if (name[0] == 0x05)
		ret_name[0] = 0xe5;
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
