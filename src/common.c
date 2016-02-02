/*
 * In The Name Of God
 * ========================================
 * [] File Name : common.c
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
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
