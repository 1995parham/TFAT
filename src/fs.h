// In The Name Of God
// ========================================
// * File Name : fs.h
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Fri 19 Dec 2014 08:21:20 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#ifndef FS_H
#define FS_H

#include "FAT.h"

fat_dir_layout_t* find(const char* path);
fat_dir_layout_t* parse_dir(fat_dir_layout_t dir);

#endif
