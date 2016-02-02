/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef USER_H
#define USER_H

#include "FAT.h"

#define TEST_FD()                                                \
        do {                                                        \
                if (fd <= 0)                                        \
                        udie("Please open valid device first");        \
        } while (0)                                                \

#define TEST_W_FD()                                                        \
        do {                                                                \
                if (!(fcntl(fd, F_GETFL) & O_RDWR))                        \
                        udie("Please open device with write permission");\
        } while (0)                                                        \


void mount(const char *dev, int wflag);

void info(void);

void cd(const char *path);

void ls(const char *dir, int show_deleted);

void chain(fat_addr_t cluster);

void fat(void);

void dump_fat(const char *path);

void test_fat(void);

void hdump(const char *dir);

void dump(const char *dir);

void delete(const char *dir);

void undelete(const char *dir);

void test(fat_addr_t cluster);

void umount(void);

#endif
