/*
 * In The Name Of God
 * ========================================
 * [] File Name : io.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef IO_H
#define IO_H

/*
 * Returns a non-zero integer if size bytes starting at
 * pos can be read without errors. Otherwise, it returns zero.
*/
int fs_test(int fd, off_t pos, int size);

#endif
