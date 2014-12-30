/*
 * In The Name Of God
 * ========================================
 * [] File Name : label.c
 *
 * [] Creation Date : 31-12-2014
 *
 * [] Last Modified : Wed 31 Dec 2014 12:49:12 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
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
