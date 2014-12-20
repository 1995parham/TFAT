// In The Name Of God
// ========================================
// * File Name : fs.h
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Sat 20 Dec 2014 01:50:00 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#ifndef FS_H
#define FS_H

#include "FAT.h"

// Set file descriptor for fs handler
void init_fs(int dev);
// Find part of path in a directory and call itselt on that directory with strtok
// if strtok return null, it return address of finded directory, please free all address and copy this address in a heap after return.
fat_dir_layout_t* search(fat_dir_layout_t* root_dir, const char* term);
// Find path in filesystem tree.
fat_dir_layout_t* find(const char* path);
// Parse directory and set dir_size to entry number and set entry it self in fat_dir_layout*.
fat_dir_layout_t* parse_dir(fat_dir_layout_t dir, int* dir_size);

#endif
