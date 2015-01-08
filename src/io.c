/*
 * In The Name Of God
 * ========================================
 * [] File Name : io.c
 *
 * [] Creation Date : 01-01-2015
 *
 * [] Last Modified : Thu 08 Jan 2015 06:37:25 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "io.h"

int fs_test(int fd, off_t pos, int size)
{
	void *buff;
	int status;

	if (lseek(fd, pos, SEEK_SET) == -1) {
		perror("status");
		return 0;
	}
	buff = malloc(size);
	status = read(fd, buff, size);
	perror("status");
	free(buff);
	return (status >= 0) ? status : 0;
}
