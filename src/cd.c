/*
 * In The Name Of God
 * ========================================
 * [] File Name : cd.c
 *
 * [] Creation Date : 29-12-2014
 *
 * [] Last Modified : Tue 30 Dec 2014 05:58:21 PM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <string.h>
#include <stdlib.h>

#include "cd.h"

char *current_path = NULL;

/*
 * Note that new_path must have '/'
 * at the end ...
 * if your string do not have '/'
 * at the end everything goes
 * wrong if you want to "parse it" ...
*/
void change_current_path(const char *new_path)
{
	if (current_path)
		free(current_path);
	current_path = malloc((strlen(new_path) + 1) * sizeof(char));
	strcpy(current_path, new_path);
}

/*
 * Note that new_path must have /
 * at the end ...
*/
void concat_current_path(const char *new_path)
{
	if (current_path) {
		int len = strlen(current_path) + strlen(new_path) + 1;

		current_path = realloc(current_path, len * sizeof(char));
		strcat(current_path, new_path);
	} else {
		int len = strlen(new_path) + 1;

		current_path = malloc(len * sizeof(char));
		strcpy(current_path, new_path);
	}
}
