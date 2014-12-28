/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.h
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Sun 28 Dec 2014 05:54:24 AM IRST
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
			udie("Please open valid device first");	\
	} while (0)						\

#define TEST_W_FD()							\
	do {								\
		if (!(fcntl(fd, F_GETFL) & O_RDWR))			\
			udie("Please open device with write permission");\
	} while (0)							\


void mount(const char *dev, int wflag);

void info(void);

void ls(const char *dir);

void chain(fat_addr_t cluster);

void fat(void);

void dump_fat(const char *path);

void test_fat(void);

void hdump(const char *dir);

void dump(const char *dir);

void umount(void);

#endif
