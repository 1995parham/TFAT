/*
 *  TFAT, Fat parser and cli
 *  Copyright (C) 2015  Parham Alvani (parham.alvani@gmail.com)
 *  Copyright (C) 2015  Elahe Jalalpour (el.jalalpour@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
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
