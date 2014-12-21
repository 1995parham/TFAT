/*
 * In The Name Of God
 * ========================================
 * [] File Name : fs.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Mon 22 Dec 2014 01:19:20 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

#include "FAT.h"
#include "fs.h"

static int fd = -1;

void init_fs(int dev)
{
	fd = dev;
	/*
	 * TODO load all filesystem tree in
	 * memory
	*/
}

struct fat_dir_layout *search(const struct fat_dir_layout *root_dir, const char *term, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		if (root_dir[i].file_size && !is_special(root_dir[i].attr)) {
			char dis_name[255];
			char *temp = get_name(root_dir[i].name);

			if (temp) {
				strcpy(dis_name, temp);
				free(temp);
			} else {
				continue;
			}
			dis_name[strlen(dis_name) + 1] = 0;
			dis_name[strlen(dis_name)] = '.';
			temp = get_extention(root_dir[i].extention);
			strcpy(dis_name + strlen(dis_name), temp);
			free(temp);

			if (!strcmp(dis_name, term))
				return &root_dir[i];
		}
		if (is_directory(root_dir[i].attr)) {
			char dis_name[255];
			char *temp = get_name(root_dir[i].name);

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
	if (dir != root_dir && dir)
		free(dir);
	return NULL;
}

/*
 * TODO add directory chain handling with realoc()
*/
struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size)
{
	if (!is_directory(dir.attr))
		return NULL;

	*dir_size = 0;
	struct fat_dir_layout *entries = malloc(512 * fat_boot.sectors_per_cluster);
	int index = 0;
	fat_addr_t cluster = dir.first_cluster;

	while (cluster) {
		lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
		read(fd, entries + index, 512 * fat_boot.sectors_per_cluster);
		*dir_size += (512 * fat_boot.sectors_per_cluster) / sizeof(struct fat_dir_layout);
		index += 512 * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}

	return entries;
}
