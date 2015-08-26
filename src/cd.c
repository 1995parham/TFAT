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
