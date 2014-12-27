/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.h
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sat 27 Dec 2014 11:23:51 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef USER_H
#define USER_H

#include "FAT.h"

#define TEST_FD()						\
	do {							\
		if (fd <= 0)					\
			die("Please open valid device first");	\
	} while (0)						\

void mount(const char *dev);

void info(void);

void ls(const char *dir);

void chain(fat_addr_t cluster);

void fat(void);

void dump_fat(const char *path);

void test_fat(void);

void hdump(const char *dir);

void dump(const char *dir);

void umount(void);

void lls(void);

#endif
