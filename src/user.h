/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.h
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sun 21 Dec 2014 03:30:08 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef USER_H
#define USER_H

#include "FAT.h"

void mount(const char *dev);

void info(void);

void ls(const char *dir);

void chain(fat_addr_t cluster);

void fat(void);

void dump_fat(const char *path);

void hdump(const char *dir);

void dump(const char *dir);

void umount(void);

#endif
