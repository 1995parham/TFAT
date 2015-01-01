/*
 * In The Name Of God
 * ========================================
 * [] File Name : io.c
 *
 * [] Creation Date : 01-01-2015
 *
 * [] Last Modified : Thu 01 Jan 2015 06:46:51 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <unistd.h>
#include <stdlib.h>

#include "io.h"

int fs_test(int fd, off_t pos, int size)
{
	void *buff;
	int status;

	lseek(fd, pos, SEEK_SET);
	buff = malloc(size);
	status = read(fd, buff, size);
	free(buff);
	return status;
}
