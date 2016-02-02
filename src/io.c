/*
 * In The Name Of God
 * ========================================
 * [] File Name : io.c
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
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
