// In The Name Of God
// ========================================
// * File Name : command.c
// 
// * Creation Date : 16-12-2014
//
// * Last Modified : Tue 16 Dec 2014 11:37:06 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "common.h"
#include "serial.h"

// Commands in this program have the following foramt
// VERB [1] [2] ...      len(VERB) <= 10
// every VERB have handler in following format 
// void command_VERB([1], [2], [3], ..) 
//


void quit_command(){
	exit(0);
}

void init_command(){
	on_serial = 2;
	init_serial();
}

void command_dispatcher(const char* command){
	char verb[10];
	sscanf(command, "%s", verb);

	if(!strcmp(verb, "quit")){
		quit_command();
	}else if(!strcmp(verb, "init")){
		init_command();
	}
}


