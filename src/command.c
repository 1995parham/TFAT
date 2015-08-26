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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "user.h"
#include "command.h"
#include "common.h"
#include "cd.h"
#include "label.h"

/*
 * Commands in this program have the following foramt
 * VERB [1] [2] ...      len(VERB) <= 10
 * every VERB have handler in following format
 * void VERB_command([1], [2], [3], ..)
*/


void quit_command(void)
{
	umount();
	free(current_path);
	free(label);
	exit(0);
}

void umount_command(void)
{
	umount();
}

void mount_command(const char *dev)
{
	mount(dev, 0);
}

void wmount_command(const char *dev)
{
	mount(dev, 1);
}

void cd_command(const char *path)
{
	cd(path);
}

void ls_command(const char *dir)
{
	ls(dir, 0);
}

void lsdel_command(const char *dir)
{
	ls(dir, 1);
}

void chain_command(uint16_t index)
{
	chain(index);
}

void fat_command(void)
{
	fat();
}

void dump_fat_command(const char *path)
{
	dump_fat(path);
}

void test_fat_command(void)
{
	test_fat();
}

void dump_command(const char *dir)
{
	dump(dir);
}

void hdump_command(const char *dir)
{
	hdump(dir);
}

void delete_command(const char *dir)
{
	delete(dir);
}

void undelete_command(const char *dir)
{
	undelete(dir);
}

void info_command(void)
{
	info();
}

void test_command(uint32_t cluster)
{
	test(cluster);
}

void show_command(char c)
{
	switch (c) {
		case 'w':
			printf("TFAT is a simple interactive fat parser\n");

	                printf("Copyright (C) 2014 ");
	                printf("Parham Alvani(parham.alvani@gmail.com)\n");

	                printf("This program is free software: ");
	                printf("you can redistribute it and/or modify\n");

	                printf("it under the terms of the ");
	                printf("GNU General Public License as published by\n");

	                printf("the Free Software Foundation, ");
	                printf("either version 3 of the License, or\n");

	                printf("(at your option) any later version.\n");

	                printf("This program is distributed in the hope");
	                printf(" that it will be useful,\n");

	                printf("but WITHOUT ANY WARRANTY; ");
	                printf("without even the implied warranty of\n");

	                printf("MERCHANTABILITY or FITNESS FOR A ");
	                printf("PARTICULAR PURPOSE.  See the\n");

	                printf("GNU General Public License for more details.\n");

	                printf("You should have received a copy of the ");
	                printf("GNU General Public License\n");

	                printf("along with this program.  ");
	                printf("If not, see <http://www.gnu.org/licenses/>.\n");
	                break;
		case 'c':
			printf("TFAT is a simple interactive fat parser\n");

	                printf("Copyright (C) 2014 ");
	                printf("Parham Alvani(parham.alvani@gmail.com)\n");

	                printf("This program is free software: ");
	                printf("you can redistribute it and/or modify\n");

	                printf("it under the terms of the ");
	                printf("GNU General Public License as published by\n");

	                printf("the Free Software Foundation, ");
	                printf("either version 3 of the License, or\n");

	                printf("(at your option) any later version.\n");

	                printf("This program is distributed in the hope");
	                printf(" that it will be useful,\n");

	                printf("but WITHOUT ANY WARRANTY; ");
	                printf("without even the implied warranty of\n");

	                printf("MERCHANTABILITY or FITNESS FOR A ");
	                printf("PARTICULAR PURPOSE.  See the\n");

	                printf("GNU General Public License for more details.\n");

	                printf("You should have received a copy of the ");
	                printf("GNU General Public License\n");

	                printf("along with this program.  ");
	                printf("If not, see <http://www.gnu.org/licenses/>.\n");
	                break;
	}
}

void command_dispatcher(const char *command)
{
	char verb[10];
	int len;

	len = sscanf(command, "%s", verb);
	if (len < 1)
		return;

	if (!strcmp(verb, "quit")) {
		quit_command();
	} else if (!strcmp(verb, "mount")) {
		char dev[MAX_BUFF];
		int len = 0;

		len = sscanf(command, "%s %s", verb, dev);
		if (len < 2) {
			printf("mount mount-device\n");
			return;
		}
		mount_command(dev);
	} else if (!strcmp(verb, "wmount")) {
		char dev[MAX_BUFF];
		int len = 0;

		len = sscanf(command, "%s %s", verb, dev);
		if (len < 2) {
			printf("wmount mount-device\n");
			return;
		}
		wmount_command(dev);
	} else if (!strcmp(verb, "umount")) {
		umount_command();
	} else if (!strcmp(verb, "cd")) {
		char path[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, path);
		if (len < 2) {
			return;
		}
		cd_command(path);
	} else if (!strcmp(verb, "ls")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			ls_command(NULL);
			return;
		}
		ls_command(dir);
	} else if (!strcmp(verb, "lsdel")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			lsdel_command(NULL);
			return;
		}
		lsdel_command(dir);
	} else if (!strcmp(verb, "chain")) {
		uint32_t index;
		int len;

		len = sscanf(command, "%s %u", verb, &index);
		if (len < 2) {
			printf("chain cluster-number\n");
			return;
		}
		chain_command(index);
	} else if (!strcmp(verb, "fat")) {
		fat_command();
	} else if (!strcmp(verb, "dump_fat")) {
		char path[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, path);
		if (len < 2) {
			printf("dump_fat file-path\n");
			return;
		}
		dump_fat_command(path);
	} else if (!strcmp(verb, "dump")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			printf("dump file-name");
			return;
		}
		dump_command(dir);
	} else if (!strcmp(verb, "hdump")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			printf("hdump file-name");
			return;
		}
		hdump_command(dir);
	} else if (!strcmp(verb, "delete")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			printf("delete file-name");
			return;
		}
		delete_command(dir);
	} else if (!strcmp(verb, "undelete")) {
		char dir[MAX_BUFF];
		int len;

		len = sscanf(command, "%s %s", verb, dir);
		if (len < 2) {
			printf("undelete file-name");
			return;
		}
		undelete_command(dir);

	} else if (!strcmp(verb, "info")) {
		info_command();
	} else if (!strcmp(verb, "show")) {
		char c;
		int len;

		len = sscanf(command, "%s %c", verb, &c);
		if (len < 2) {
			printf("show w|c\n");
			return;
		}
		show_command(c);
	} else if (!strcmp(verb, "test_fat")) {
		test_fat_command();
	} else if (!strcmp(verb, "test")) {
		uint32_t index;
		int len;

		len = sscanf(command, "%s %u", verb, &index);
		if (len < 2) {
			printf("test cluster-number\n");
			return;
		}
		test_command(index);
	} else {
		printf("404 Not Found\n");
	}
}
