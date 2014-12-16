// In The Name Of God
// ========================================
// * File Name : command.c
// 
// * Creation Date : 16-12-2014
//
// * Last Modified : Wed 17 Dec 2014 02:08:13 AM IRST
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
	}
}


