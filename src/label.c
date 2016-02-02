/*
 * In The Name Of God
 * ========================================
 * [] File Name : label.c
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include <string.h>
#include <stdlib.h>

#include "label.h"

char *label = NULL;

void change_label(const char *new_label)
{
	if (!new_label)
		return;
	if (label)
		free(label);
	label = malloc((strlen(new_label) + 1) * sizeof(char));
	strcpy(label, new_label);
}
