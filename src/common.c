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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

char *rtrim(char *string)
{
	char *original = string + strlen(string);
	int i = strlen(string);

	while (i-- && *(--original) == ' ');
	if (i != -1)
		*(original + 1) = 0;
	else
		*(original) = 0;
	return string;
}

void sdie(const char *fmt, ...)
{
	char buf[MAX_BUFF];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	perror(buf);
	exit(EXIT_FAILURE);
}

void udie(const char *fmt, ...)
{
	char buf[MAX_BUFF];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	int len = strlen(buf);
	buf[len] = '\n';
	buf[len + 1] = 0;

	fputs(buf, stderr);
	exit(EXIT_FAILURE);
}
