/*
 * In The Name Of God
 * ========================================
 * [] File Name : fs.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Thu 08 Jan 2015 06:38:03 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "FAT.h"
#include "fs.h"
#include "io.h"

static int fd = -1;

void init_fs(int dev)
{
	fd = dev;
}

struct fat_dir_layout *search(const struct fat_dir_layout *root_dir, const char *term, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		if (root_dir[i].file_size && !is_special(root_dir[i].attr)) {
			char dis_name[255];
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

			if (!strcmp(dis_name, term))
				return &root_dir[i];
		}
		if (is_directory(root_dir[i].attr)) {
			char dis_name[255];
			char *temp = get_name(root_dir[i].name,
					root_dir[i].case_information);

			if (temp) {
				strcpy(dis_name, temp);
				free(temp);
			} else {
				continue;
			}

			if (!strcmp(dis_name, term))
				return &root_dir[i];
		}
	}
	return NULL;

}

struct fat_dir_layout *find(const char *path)
{
	char *str = malloc((strlen(path) + 1) * sizeof(char));
	char *token = NULL;
	struct fat_dir_layout *dir = root_dir;
	struct fat_dir_layout *file = malloc(sizeof(struct fat_dir_layout));
	int size = fat_boot.root_entry_count;

	strcpy(str, path);
	token = strtok(str, "/");

	while (token) {
		struct fat_dir_layout *tmp = search(dir, token, size);

		if (tmp)
			*file = *tmp;
		else
			goto error;

		if (dir != root_dir)
			free(dir);

		token = strtok(NULL, "/");
		if (!token)
			break;

		dir = parse_dir(*file, &size);
		if (!dir)
			goto error;
	}
	free(str);
	return file;
error:
	free(str);
	free(file);
	if (dir != root_dir && dir)
		free(dir);
	return NULL;
}

char *get_parent_path(const char *path)
{
	char *ret = malloc(strlen(path) * sizeof(char) + 1);

	strcpy(ret, path);
	if (ret[strlen(ret) - 1] == '/')
		ret[strlen(ret) - 1] = 0;

	int len = strlen(ret);

	while (ret[len] != '/' && len)
		len--;
	ret[len] = '/';
	ret[len + 1] = 0;

	return ret;
}

char *fix_deleted_char(const char *path, char *c)
{
	char *ret = malloc(strlen(path) * sizeof(char) + 1);

	strcpy(ret, path);
	if (ret[strlen(ret) - 1] == '/')
		ret[strlen(ret) - 1] = 0;

	int len = strlen(ret);

	while (ret[len] != '/' && len)
		len--;
	if (len) {
		*c = ret[len + 1];
		ret[len + 1] = 0xE5;
	} else {
		*c = ret[len];
		ret[len] = 0xE5;
	}

	return ret;
}

/*
 * TODO add directory chain handling with realoc()
*/
struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size)
{
	if (!is_directory(dir.attr))
		return NULL;

	*dir_size = 0;
	struct fat_dir_layout *entries = NULL;
	fat_addr_t cluster = first_cluster(dir);

	int index = 0;
	entries = malloc(SECTOR * fat_boot.sectors_per_cluster);

	while (cluster) {
		lseek(fd, cluster_to_sector(cluster), SEEK_SET);
		read(fd, entries + index, SECTOR * fat_boot.sectors_per_cluster);
		*dir_size += (SECTOR * fat_boot.sectors_per_cluster) / sizeof(struct fat_dir_layout);
		index += SECTOR * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}

	return entries;
}

/*
 * TODO add directory chain handling
*/
void write_dir(struct fat_dir_layout dir, const struct fat_dir_layout *entries, int dir_size)
{
	if (!is_directory(dir.attr))
		return;

	fat_addr_t cluster = first_cluster(dir);
	int index = 0;

	while (cluster) {
		lseek(fd, cluster_to_sector(cluster), SEEK_SET);
		write(fd, entries + index, SECTOR * fat_boot.sectors_per_cluster);
		dir_size -= (SECTOR * fat_boot.sectors_per_cluster) / sizeof(struct fat_dir_layout);
		index += SECTOR * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}
}

int test_cluster(fat_addr_t index)
{
	fat_addr_t sector = cluster_to_sector(index);
	int i = 0;

	for (i = 0; i < fat_boot.sectors_per_cluster; i++) {
		fprintf(stderr, "sector %u in cluster %u ", i, index);
		if (!fs_test(fd, sector + i * SECTOR, SECTOR))
			return 0;
	}
	return 1;
}
