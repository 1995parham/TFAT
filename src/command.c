// In The Name Of God
// ========================================
// * File Name : command.c
// 
// * Creation Date : 16-12-2014
//
// * Last Modified : Thu 18 Dec 2014 01:22:46 AM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "command.h"
#include "common.h"

// Commands in this program have the following foramt
// VERB [1] [2] ...      len(VERB) <= 10
// every VERB have handler in following format 
// void VERB_command([1], [2], [3], ..) 
//


void quit_command(){
	exit(0);
}

void mount_command(const char* dev){
	mount(dev);
}

void ls_command(const char* dir){
	ls(dir);
}

void show_command(char c){
	switch(c){
		case 'w':
			printf("TFAT is a simple interactive fat parser\n");
    			printf("Copyright (C) 2014  Parham Alvani(parham.alvani@gmail.com)\n");
			printf("This program is free software: you can redistribute it and/or modify\n");
			printf("it under the terms of the GNU General Public License as published by\n");
			printf("the Free Software Foundation, either version 3 of the License, or\n");
			printf("(at your option) any later version.\n");

			printf("This program is distributed in the hope that it will be useful,\n");
			printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
			printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
			printf("GNU General Public License for more details.\n");

    			printf("You should have received a copy of the GNU General Public License\n");
    			printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
			break;
		case 'c':
			printf("TFAT is a simple interactive fat parser\n");
    			printf("Copyright (C) 2014  Parham Alvani(parham.alvani@gmail.com)\n");
			printf("This program is free software: you can redistribute it and/or modify\n");
			printf("it under the terms of the GNU General Public License as published by\n");
			printf("the Free Software Foundation, either version 3 of the License, or\n");
			printf("(at your option) any later version.\n");

			printf("This program is distributed in the hope that it will be useful,\n");
			printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
			printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
			printf("GNU General Public License for more details.\n");

    			printf("You should have received a copy of the GNU General Public License\n");
    			printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
			break;
	}
}

void command_dispatcher(const char* command){
	char verb[10];
	sscanf(command, "%s", verb);

	if(!strcmp(verb, "quit")){
		quit_command();
	}else if(!strcmp(verb, "mount")){
		char dev[MAX_BUFF];
		sscanf(command, "%s %s", verb, dev);
		mount_command(dev);
	}else if(!strcmp(verb, "ls")){
		char dir[MAX_BUFF];
		sscanf(command, "%s %s", verb, dir);
		ls_command(dir);
	}else if(!strcmp(verb, "show")){
		char c;
		sscanf(command, "%s %c", verb, &c);
		show_command(c);
	}
}


