/*
 * In The Name Of God
 * ========================================
 * [] File Name : fs.h
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sun 21 Dec 2014 03:34:20 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef FS_H
#define FS_H

#include "FAT.h"
/*
 * Set file descriptor for fs handler
*/
void init_fs(int dev);

/*
 * Find part of path in a directory and call itselt on that directory with strtok
 * if strtok return null, it return address of finded directory, please free all
 * address and copy this address in a heap after return.
*/
struct fat_dir_layout *search(struct fat_dir_layout *root_dir, const char *term);

/*
 * Find path in filesystem tree.
*/
struct fat_dir_layout *find(const char *path);

/*
 * Parse directory and set dir_size to entry number and
 * set entry itself in fat_dir_layout*
*/
struct fat_dir_layout *parse_dir(struct fat_dir_layout dir, int *dir_size);

#endif
