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
#ifndef FS_H
#define FS_H

#include "FAT.h"

/*
 * Set file descriptor for fs handler
*/
void init_fs(int dev);

/*
 * Find part of path in a directory and return it's address
 * if path not found it return NULL
 * you should free directory list and store returned address
 * elsewhere.
*/
struct fat_dir_layout *search(const struct fat_dir_layout *root_dir,
	const char *term, int size);

/*
 * Find path in filesystem tree.
 * please free returned address.
*/
struct fat_dir_layout *find(const char *path);

/*
 * Return parent path for
 * given path, for example
 * $$$/A/B ==> $$$/A/
*/
char *get_parent_path(const char *path);

/*
 * Change first character after last '/'
 * to 0xE5 because user cannot enter 0xE5 !!
 * so we enter it for him !!!
 * note that it set character c to character
 * it changed.
*/
char *fix_deleted_char(const char *path, char *c);

/*
 * Parse directory and set dir_size to entry number and
 * set entry itself in fat_dir_layout*
*/
struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size);

/*
 * Write entries at first cluster of dir
*/
void write_dir(struct fat_dir_layout dir,
	const struct fat_dir_layout *entries, int dir_size);

/*
 * Check given cluster for errors
 * if there is a error it returns
 * zero otherwise it returns non
 * zero integer
*/
int test_cluster(fat_addr_t index);

#endif
