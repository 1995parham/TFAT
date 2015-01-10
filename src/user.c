/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sat 10 Jan 2015 03:49:55 PM IRST
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
#include "cd.h"
#include "label.h"

static int fd;

void mount(const char *dev, int wflag)
{
	if (wflag)
		fd = open(dev, O_RDWR);
	else
		fd = open(dev, O_RDONLY);

	if (fd <= 0)
		sdie("Cannot open %s", dev);

	init_fat(fd);
	init_fs(fd);

	if (fat_boot.media_type == 0x00) {
		free_fat();
		udie("There is nothing on %s", dev);
	}

	info();

	change_current_path("/");

	char *new_label = get_label();

	if (new_label) {
		change_label(new_label);
		free(new_label);
	}
}


void info(void)
{
	TEST_FD();
	if (FATN == 1)
		printf("FAT16\n");
	else
		printf("FAT32\n");

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

	printf("Reserved Sectors: %hu\n",
			fat_boot.reserved_sector_count);

	printf("FAT Tables: %hhu\n",
			fat_boot.table_count);

	printf("Table Size: %u\n", table_size());

	printf("Total Sectors: %u\n", total_sectors());

	uint64_t total_size = total_sectors();
	total_size *= fat_boot.bytes_per_sector;
	total_size /= (1024 * 1024);
	printf("Total Size: %lu MB\n", total_size);
}

/*
 * This is a helper funtion for printing
 * list directory
 *
 * Subdirectory: Indicates that the cluster-chain
 * associated with this entry gets
 * interpreted as subdirectory instead of as a file,
 * subdirectories have a filesize entry of zero.
 *
 * if you want to print deleted directory you must
 * set show_deleted = 1
*/
static void list(struct fat_dir_layout *root_dir, int size, int show_deleted)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		if (is_deleted(root_dir[i].name) && !show_deleted)
			continue;
		if (!is_deleted(root_dir[i].name) && is_lfn(root_dir[i].attr))
			lfn_add_slot(&root_dir[i], i);
		if (!is_directory(root_dir[i].attr) && !is_special(root_dir[i].attr)) {
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
			printf("%s %s %4u %12s %4hX %s\n", dis_attr, dis_time
					, root_dir[i].file_size, dis_name,
					first_cluster(root_dir[i]), dis_full_name);
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
			printf("%s %s ---- %12s %4hX %s\n", dis_attr,
					dis_time, dis_name,
					first_cluster(root_dir[i]), dis_full_name);
		}
	}
}

/*
 * This is a helper function
 * for coverting relative address
 * to complete address
*/
char *address_convertor(const char *path)
{
	int len = 0;
	char *ret = NULL;

	if (!path) {
		len = strlen(current_path) + 1;
		ret = malloc(len * sizeof(char));

		strcpy(ret, current_path);
	} else if (path[0] == '.' && path[1] == '.') {
		int seek = 2;

		if (path[2] == '/')
			seek = 3;

		ret = get_parent_path(current_path);
		len = strlen(ret) + strlen(path);
		ret = realloc(ret, len * sizeof(char));
		strcat(ret, path + seek);

		if (ret[strlen(ret) - 1] != '/') {
			ret[strlen(ret) + 1] = 0;
			ret[strlen(ret)] = '/';
		}
	} else if (path[0] == '.') {
		int seek = 1;

		if (path[1] == '/')
			seek = 2;

		len = strlen(current_path) + 1;
		ret = malloc(len * sizeof(char));
		strcpy(ret, current_path);
		len = strlen(ret) + strlen(path);
		ret = realloc(ret, len * sizeof(char));
		strcat(ret, path + seek);

		if (ret[strlen(ret) - 1] != '/') {
			ret[strlen(ret) + 1] = 0;
			ret[strlen(ret)] = '/';
		}
	} else if (path[0] != '/') {
		/* We want add / at end if necessary */
		len = strlen(current_path) + strlen(path) + 2;
		ret = malloc(len * sizeof(char));

		strcpy(ret, current_path);
		strcat(ret, path);

		if (ret[strlen(ret) - 1] != '/') {
			ret[strlen(ret) + 1] = 0;
			ret[strlen(ret)] = '/';
		}
	} else {
		/* We want add / at end if necessary */
		len = strlen(path) + 2;
		ret = malloc(len * sizeof(char));

		strcpy(ret, path);

		if (ret[strlen(ret) - 1] != '/') {
			ret[strlen(ret) + 1] = 0;
			ret[strlen(ret)] = '/';
		}
	}

	return ret;
}

void cd(const char *path)
{
	struct fat_dir_layout *dir;
	char *npath = NULL;

	npath = address_convertor(path);
	if (!strcmp(npath, "/")) {
		change_current_path(npath);
		free(npath);
	} else {

		dir = find(npath);

		if (dir == NULL) {
			printf("Folder %s Not Found\n", npath);
			free(npath);
			return;
		}
		if (!is_directory(dir->attr)) {
			printf("%s is Not Directory\n", npath);
		free(npath);
		free(dir);
		return;
		}
		change_current_path(npath);

		free(dir);
		free(npath);
	}
}

/*
 * First if our dir == '/',
 * we need to list root_dir
 * so we cannot use our find
 * function so we do it manually
*/
void ls(const char *dir, int show_deleted)
{
	TEST_FD();

	char *ndir = NULL;

	ndir = address_convertor(dir);

	if (!strcmp(ndir, "/")) {
		list(root_dir, fat_boot.root_entry_count, show_deleted);
	} else {
		int size;
		struct fat_dir_layout *folder = find(ndir);
		if (folder == NULL) {
			printf("Folder %s Not Found\n", ndir);
			free(ndir);
			return;
		}
		struct fat_dir_layout *root_dir = parse_dir(*folder, &size);
		if (root_dir == NULL) {
			printf("Folder %s Not Found\n", ndir);
			free(folder);
			free(ndir);
			return;
		}

		list(root_dir, size, show_deleted);

		free(folder);
		free(root_dir);
	}

	free(ndir);
}

void hdump(const char *dir)
{
	TEST_FD();

	char *ndir = NULL;

	ndir = address_convertor(dir);

	struct fat_dir_layout *file = find(ndir);
	if (file == NULL) {
		printf("File %s Not Found\n", ndir);
		free(ndir);
		return;
	}
	fat_addr_t cluster = first_cluster(*file);
	unsigned int size = file->file_size;
	printf("File: %s, Size: %d\n", ndir, size);
	printf("================####====================\n");
	while (cluster) {
		if (size < fat_boot.sectors_per_cluster * SECTOR) {
			uint8_t buff[size];
			int j = 0;

			lseek(fd, cluster_to_sector(cluster), SEEK_SET);
			read(fd, buff, size);

			for (j = 0; j < size; j++)
				printf("%02X ", buff[j]);
		} else {
			uint8_t buff[SECTOR * fat_boot.sectors_per_cluster];
			int j = 0;

			lseek(fd, cluster_to_sector(cluster), SEEK_SET);
			read(fd, buff, SECTOR * fat_boot.sectors_per_cluster);

			for (j = 0; j < SECTOR *
					fat_boot.sectors_per_cluster; j++)
				printf("%02X ", buff[j]);
			size -= SECTOR * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
	free(file);
	free(ndir);
}

void dump(const char *dir)
{
	TEST_FD();

	char *ndir = NULL;

	ndir = address_convertor(dir);

	struct fat_dir_layout *file = find(ndir);

	if (file == NULL) {
		printf("File %s Not Found\n", ndir);
		free(ndir);
		return;
	}
	fat_addr_t cluster = first_cluster(*file);
	unsigned int size = file->file_size;

	printf("File: %s, Size: %d\n", ndir, size);
	printf("================####====================\n");
	while (cluster) {
		if (size < fat_boot.sectors_per_cluster * SECTOR) {
			uint8_t buff[size];
			int j = 0;

			lseek(fd, cluster_to_sector(cluster), SEEK_SET);
			read(fd, buff, size);

			for (j = 0; j < size; j++)
				printf("%c", buff[j]);
		} else {
			uint8_t buff[SECTOR * fat_boot.sectors_per_cluster];
			int j = 0;

			lseek(fd, cluster_to_sector(cluster), SEEK_SET);
			read(fd, buff, SECTOR * fat_boot.sectors_per_cluster);

			for (j = 0; j < SECTOR *
					fat_boot.sectors_per_cluster; j++)
				printf("%c", buff[j]);
			size -= SECTOR * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
	free(file);
	free(ndir);
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

	for (i = 0; i < table_size(); i++) {
		if (i % 10 == 0) {
			printf("Do you want to continue ?(Y/n)");

			char c = getchar();

			getchar();
			if (c != 'Y')
				return;
		}
		printf("%X <--> %X\n", i, get_cluster(i));
	}
}

void dump_fat(const char *path)
{
	TEST_FD();

	int file = open(path, O_WRONLY | O_CREAT, 0644);

	if (file <= 0)
		sdie("cannot open %s", path);
	fat_addr_t i = 0;

	for (i = 0; i < table_size(); i++) {
		fat_addr_t value = get_cluster(i);
		write(file, &value, (FATN == 1) ? 2 : 4);
	}
	close(file);
}

void test_fat(void)
{
	TEST_FD();

	fat_addr_t i = 0;

	for (i = 0; i < table_size(); i++)
		if (get_cluster(i) != get_cluster_bak(i))
			printf("We have error on entry %u, %X != %X", i,
					get_cluster(i), get_cluster_bak(i));
}

void delete(const char *dir)
{
	TEST_FD();
	TEST_W_FD();

	char *ndir = address_convertor(dir);
	char *pndir = get_parent_path(ndir);

	int size = 0;
	struct fat_dir_layout *folder = NULL;
	struct fat_dir_layout *file = find(ndir);
	struct fat_dir_layout *parent_file = NULL;

	if (strcmp(pndir, "/")) {
		parent_file = find(pndir);
		folder = parse_dir(*parent_file, &size);
	} else {
		folder = root_dir;
		size = fat_boot.root_entry_count;
	}
	free(ndir);
	free(pndir);

	if (file == NULL) {
		printf("File %s Not Found\n", dir);
		return;
	}
	fat_addr_t cluster = first_cluster(*file);

	while (cluster) {
		fat_addr_t cluster_bak = cluster;

		cluster = next_cluster(cluster);
		printf("Make cluster number %hu zero\n", cluster_bak);
		change_cluster(cluster_bak, 0x00000000);
	}

	int i = 0;

	for (i = 0; i < size; i++) {
		if (!memcmp(&folder[i], file, sizeof(struct fat_dir_layout))) {
			if (parent_file) {
				folder[i].name[0] = 0xE5;
				write_dir(*parent_file, folder, size);
			} else {
				folder[i].name[0] = 0xE5;
			}
		}
	}

	if (parent_file) {
		free(folder);
		free(parent_file);
	}
	free(file);
}

void undelete(const char *dir)
{
	TEST_FD();
	TEST_W_FD();

	char deleted_char;
	char *ddir = fix_deleted_char(dir, &deleted_char);
	char *ndir = address_convertor(ddir);
	char *pndir = get_parent_path(ndir);

	int size = 0;
	struct fat_dir_layout *folder = NULL;
	struct fat_dir_layout *file = find(ndir);
	struct fat_dir_layout *parent_file = NULL;

	if (file == NULL) {
		printf("File %s Not Found\n", dir);
		free(ddir);
		free(ndir);
		free(pndir);
		return;
	}

	if (strcmp(pndir, "/")) {
		parent_file = find(pndir);
		folder = parse_dir(*parent_file, &size);
	} else {
		folder = root_dir;
		size = fat_boot.root_entry_count;
	}
	free(ddir);
	free(ndir);
	free(pndir);

	fat_addr_t cluster = first_cluster(*file);

	printf("Make cluster number %hu unzero\n", cluster);
	change_cluster(cluster, 0x0FFFFFFF);

	int i = 0;

	for (i = 0; i < size; i++) {
		if (!memcmp(&folder[i], file, sizeof(struct fat_dir_layout))) {
			if (parent_file) {
				folder[i].name[0] = deleted_char;
				write_dir(*parent_file, folder, size);
			} else {
				folder[i].name[0] = deleted_char;
			}
		}
	}

	if (parent_file) {
		free(folder);
		free(parent_file);
	}
	free(file);
}

void test(fat_addr_t cluster)
{
	TEST_FD();

	if (!test_cluster(cluster))
		printf("%u : Bad Cluster\n", cluster);
	else
		printf("%u : Good Cluster\n", cluster);
}

void umount(void)
{
	if (fd > 0) {
		if (fcntl(fd, F_GETFL) & O_RDWR)
			write_fat(fd);
		close(fd);
		free_fat();
		change_current_path("-");
		change_label("NOTHING");
		fd = 0;
	}
}
