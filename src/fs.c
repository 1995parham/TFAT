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
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "FAT.h"
#include "fs.h"
#include "io.h"

static int fd = -1;

void init_fs(int dev)
{
	fd = dev;
}

struct fat_dir_layout *search(const struct fat_dir_layout *root_dir,
	const char *term, int size)
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
			temp = get_extension(root_dir[i].extention,
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
		ret[len + 1] = '?';
	} else {
		*c = ret[len];
		ret[len] = '?';
	}

	return ret;
}

struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size)
{
	if (!is_directory(dir.attr))
		return NULL;

	*dir_size = 0;
	struct fat_dir_layout *entries = NULL;
	int nr = 1;
	fat_addr_t cluster = first_cluster(dir);

	while (cluster) {
		entries = realloc(entries,
			SECTOR * fat_boot.sectors_per_cluster * nr);
		lseek(fd, cluster_to_sector(cluster), SEEK_SET);
		read(fd, entries + *dir_size,
			SECTOR * fat_boot.sectors_per_cluster);
		*dir_size += (SECTOR * fat_boot.sectors_per_cluster) /
		             sizeof(struct fat_dir_layout);
		nr++;
		cluster = next_cluster(cluster);
	}

	return entries;
}

void write_dir(struct fat_dir_layout dir, const struct fat_dir_layout *entries,
	int dir_size)
{
	if (!is_directory(dir.attr))
		return;

	fat_addr_t cluster = first_cluster(dir);
	int index = 0;

	while (cluster) {
		lseek(fd, cluster_to_sector(cluster), SEEK_SET);
		write(fd, entries + index,
			SECTOR * fat_boot.sectors_per_cluster);
		dir_size -= (SECTOR * fat_boot.sectors_per_cluster) /
		            sizeof(struct fat_dir_layout);
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
