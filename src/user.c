/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sat 27 Dec 2014 12:44:57 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>

#include "FAT.h"
#include "common.h"
#include "fs.h"
#include "user.h"
#include "lfn.h"

static int fd;

void mount(const char *dev)
{
	fd = open(dev, O_RDONLY);

	if (fd <= 0)
		die("Cannot open %s", dev);

	init_fat(fd);
	init_fs(fd);

	if (fat_boot.media_type == 0x00) {
		free_fat();
		die("There is nothing on %s", dev);
	}

	info();
}


void info(void)
{
	TEST_FD();

	printf("Boot Jump: %2X %2X %2X\n",
			fat_boot.bootjmp[0],
			fat_boot.bootjmp[1],
			fat_boot.bootjmp[2]);

	printf("OEM Media: %8s\n",
			fat_boot.oem_name);

	printf("Media Type: %2X\n",
			fat_boot.media_type);

	printf("Bytes Per Sector: %hu\n",
			fat_boot.bytes_per_sector);

	printf("Sectors Per Track: %hu\n",
			fat_boot.sectors_per_track);

	printf("Sectors Per Cluster: %hu\n",
			fat_boot.sectors_per_cluster);

	printf("Root Entry Count: %hu\n",
			fat_boot.root_entry_count);

	printf("Reserved Sectors: %hu\n",
			fat_boot.reserved_sector_count);

	printf("FAT Tables: %hhu\n",
			fat_boot.table_count);

	printf("Table Size: %hu\n",
			fat_boot.table_size_16);

	printf("Total Sectors: %hu\n",
			fat_boot.total_sectors_16);

	printf("Total Size: %u KB\n",
			fat_boot.total_sectors_16 *
			fat_boot.bytes_per_sector / 1024);

	fat_addr_t root_cluster = first_data_sector();

	printf("Root Cluster Sector: %hu\n",
			root_cluster);
}

/*
 * This is a helper funtion for printing
 * list directory
 *
 * Subdirectory: Indicates that the cluster-chain
 * associated with this entry gets
 * interpreted as subdirectory instead of as a file,
 * subdirectories have a filesize entry of zero.
*/
void list(struct fat_dir_layout *root_dir, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		if (is_deleted(root_dir[i].name))
			continue;
		if (is_lfn(root_dir[i].attr))
			lfn_add_slot(&root_dir[i], i);
		if (root_dir[i].file_size && !is_special(root_dir[i].attr)) {
			char dis_name[255];
			char dis_time[255];
			char dis_attr[255];
			char dis_full_name[1024] = "";

			char *temp = get_name(root_dir[i].name,
					root_dir[i].case_information);

			if (temp) {
				strcpy(dis_name, temp);
				free(temp);
			} else {
				continue;
			}
			dis_name[strlen(dis_name) + 1] = 0;
			dis_name[strlen(dis_name)] = '.';
			temp = get_extention(root_dir[i].extention,
					root_dir[i].case_information);
			strcpy(dis_name + strlen(dis_name), temp);
			free(temp);
			temp = get_attr(root_dir[i].attr);
			strcpy(dis_attr, temp);
			free(temp);
			int unknown;
			temp = lfn_get(&root_dir[i], &unknown);
			if (temp) {
				strcpy(dis_full_name, temp);
				free(temp);
			}

			struct tm file_tm = create_time(root_dir[i].create_time, root_dir[i].create_date);

			strftime(dis_time, 255, "%b %d %T %Y", &file_tm);
			printf("%s %s %4u %12s %4hX %s\n", dis_attr, dis_time, root_dir[i].file_size, dis_name,
					root_dir[i].first_cluster, dis_full_name);
		}
		if (is_directory(root_dir[i].attr)) {
			char dis_name[255];
			char dis_time[255];
			char dis_attr[255];
			char dis_full_name[1024] = "";

			char *temp = get_name(root_dir[i].name,
					root_dir[i].case_information);

			if (temp) {
				strcpy(dis_name, temp);
				free(temp);
			} else {
				continue;
			}
			temp = get_attr(root_dir[i].attr);
			strcpy(dis_attr, temp);
			free(temp);
			int unknown;
			temp = lfn_get(&root_dir[i], &unknown);
			if (temp) {
				strcpy(dis_full_name, temp);
				free(temp);
			}


			struct tm file_tm = create_time(root_dir[i].create_time, root_dir[i].create_date);

			strftime(dis_time, 255, "%b %d %T %Y", &file_tm);
			printf("%s %s ---- %12s %4hX %s\n", dis_attr, dis_time, dis_name,
					root_dir[i].first_cluster, dis_full_name);
		}
	}
}

/*
 * First if our dir == '/',
 * we need to list root_dir
 * so we cannot use our find
 * function so we do it manually
*/
void ls(const char *dir)
{
	TEST_FD();

	if (dir[0] != '/') {
		printf("Invalid path\n");
		return;
	}

	if (!strcmp(dir, "/")) {
		list(root_dir, fat_boot.root_entry_count);
	} else {
		int size;
		struct fat_dir_layout *folder = find(dir);
		if (folder == NULL) {
			printf("Folder %s Not Found\n", dir);
			return;
		}
		struct fat_dir_layout *root_dir = parse_dir(*folder, &size);
		if (root_dir == NULL) {
			printf("Folder %s Not Found\n", dir);
			return;
		}

		list(root_dir, size);

		free(folder);
		free(root_dir);
	}
}

void hdump(const char *dir)
{
	TEST_FD();

	if (dir[0] != '/') {
		printf("Invalid path\n");
		return;
	}

	struct fat_dir_layout *file = find(dir);
	if (file == NULL) {
		printf("File %s Not Found\n", dir);
		return;
	}
	fat_addr_t cluster = file->first_cluster;
	unsigned int size = file->file_size;
	printf("File: %s, Size: %d\n", dir, size);
	printf("================####====================\n");
	while (cluster) {
		if (size < fat_boot.sectors_per_cluster * SECTOR) {
			uint8_t buff[size];
			lseek(fd, SECTOR * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, size);

			int j = 0;

			for (j = 0; j < size; j++)
				printf("%02X ", buff[j]);
		} else {
			uint8_t buff[SECTOR * fat_boot.sectors_per_cluster];
			lseek(fd, SECTOR * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, SECTOR * fat_boot.sectors_per_cluster);

			int j = 0;

			for (j = 0; j < SECTOR * fat_boot.sectors_per_cluster; j++)
				printf("%02X ", buff[j]);
			size -= SECTOR * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
	free(file);
}

void dump(const char *dir)
{
	TEST_FD();

	if (dir[0] != '/') {
		printf("Invalid path\n");
		return;
	}
	struct fat_dir_layout *file = find(dir);

	if (file == NULL) {
		printf("File %s Not Found\n", dir);
		return;
	}
	fat_addr_t cluster = file->first_cluster;
	unsigned int size = file->file_size;

	printf("File: %s, Size: %d\n", dir, size);
	printf("================####====================\n");
	while (cluster) {
		if (size < fat_boot.sectors_per_cluster * SECTOR) {
			uint8_t buff[size];

			lseek(fd, SECTOR * fat_boot.sectors_per_cluster *
					(cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, size);

			int j = 0;

			for (j = 0; j < size; j++)
				printf("%c", buff[j]);
		} else {
			uint8_t buff[SECTOR * fat_boot.sectors_per_cluster];

			lseek(fd, SECTOR * fat_boot.sectors_per_cluster *
					(cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, SECTOR * fat_boot.sectors_per_cluster);

			int j = 0;

			for (j = 0; j < SECTOR *
					fat_boot.sectors_per_cluster; j++)
				printf("%c", buff[j]);
			size -= SECTOR * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
	free(file);
}

void chain(fat_addr_t cluster)
{
	TEST_FD();

	while (cluster) {
		if (next_cluster(cluster) != 0)
			printf("%hu -> ", cluster);
		else
			printf("%hu\n", cluster);
		cluster = next_cluster(cluster);
	}
}

void fat(void)
{
	TEST_FD();

	fat_addr_t i = 0;

	for (i = 0; i < fat_boot.table_size_16 * (SECTOR / 2); i++) {
		if (i % 10 == 0) {
			printf("Do you want to continue ?(Y/n)");

			char c = getchar();

			getchar();
			if (c != 'Y')
				return;
		}
		printf("%hX <--> %hX\n", i, fat_table[i]);
	}
}

void dump_fat(const char *path)
{
	TEST_FD();

	int file = open(path, O_WRONLY | O_CREAT, 0644);

	if (file <= 0)
		die("cannot open %s", path);
	write(file, fat_table, SECTOR * fat_boot.table_size_16);
	close(file);
}

void test_fat(void)
{
	TEST_FD();

	fat_addr_t i = 0;

	for (i = 0; i < fat_boot.table_size_16 * (SECTOR / 2); i++)
		if (fat_table[i] != fat_table_bak[i])
			printf("We have error on entry %d, %hX != %hX", i, fat_table[i], fat_table_bak[i]);
}

void umount(void)
{
	if (fd > 0) {
		close(fd);
		free_fat();
		fd = 0;
	}
}
