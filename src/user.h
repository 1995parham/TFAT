// In The Name Of God
// ========================================
// * File Name : user.h
// 
// * Creation Date : 17-12-2014
//
// * Last Modified : Fri 19 Dec 2014 02:14:52 AM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#ifndef USER_H
#define USER_H

#include "FAT.h"

void mount(const char* dev);
void ls(const char* dir);
void chain(fat_addr_t cluster);
void fat();
void dump_fat();
void umount();

#endif
