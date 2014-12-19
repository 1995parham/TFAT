// In The Name Of God
// ========================================
// * File Name : fs.h
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Fri 19 Dec 2014 11:04:01 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#ifndef FS_H
#define FS_H

#include "FAT.h"

// Find path in filesystem tree.
fat_dir_layout_t* find(const char* path);
// Parse directory and set dir_size to entry number and set entry it self in fat_dir_layout*.
fat_dir_layout_t* parse_dir(fat_dir_layout_t dir, int* dir_size);

#endif
