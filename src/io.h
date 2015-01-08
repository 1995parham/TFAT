/*
 * In The Name Of God
 * ========================================
 * [] File Name : io.h
 *
 * [] Creation Date : 01-01-2015
 *
 * [] Last Modified : Thu 08 Jan 2015 06:13:37 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef IO_H
#define IO_H

/*
 * Returns a non-zero integer if size bytes starting at
 * pos can be read without errors. Otherwise, it returns zero.
*/
int fs_test(int fd, off_t pos, int size);

#endif
