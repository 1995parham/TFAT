/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 21-12-2014
 *
 * [] Last Modified : Mon 29 Dec 2014 11:34:27 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"
#include "common.h"
#include "cd.h"

int main(int argc, char *argv[])
{
	printf("TFAT version 01, Copyright (C) 2015 Parham Alvani(parham.alvani@gmail.com)\n");
	printf("TFAT comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions; type `show c' for details.\n");
	printf("\n");

	char *command;
	char prompt[MAX_BUFF];

	change_current_path("-");

	while (true) {
		sprintf(prompt, "TFAT> [%s] ", current_path);
		command = readline(prompt);
		if (command && *command)
			add_history(command);
		command_dispatcher(command);
		free(command);
	}
}
