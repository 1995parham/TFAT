/*
 * In The Name Of God
 * ========================================
 * [] File Name : fs.h
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Mon 22 Dec 2014 12:06:12 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef FS_H
#define FS_H

#include "FAT.h"

/*
 * Set file descriptor for fs handler
 * TODO create tree for directories
*/
void init_fs(int dev);

/*
 * Find part of path in a directory and return it's address
 * if path not found it return NULL
 * you shoud free directory list and store returned address
 * elsewhere.
*/
struct fat_dir_layout *search(const struct fat_dir_layout *root_dir, const char *term, int size);

/*
 * Find path in filesystem tree.
 * please free returned address.
*/
struct fat_dir_layout *find(const char *path);

/*
 * Parse directory and set dir_size to entry number and
 * set entry itself in fat_dir_layout*
*/
struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size);

#endif
