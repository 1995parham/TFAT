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
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"
#include "common.h"
#include "cd.h"
#include "label.h"

int main(int argc, char *argv[])
{
	printf("TFAT version 01\n");
	printf("Copyright (C) 2015 Parham Alvani(parham.alvani@gmail.com)\n");
	printf("Copyright (C) 2015 Elahe Jalalpour(el.jalalpour@gmail.com)\n");
	printf("TFAT comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions; type `show c' for details.\n");
	printf("\n");

	char *command;
	char prompt[MAX_BUFF];

	change_current_path("-");
	change_label("NOTHING");

	while (true) {
		sprintf(prompt, "%s@TFAT {%s} [%s] $ ", getlogin(), label,
			current_path);
		command = readline(prompt);
		if (command && *command)
			add_history(command);
		command_dispatcher(command);
		free(command);
	}
}
