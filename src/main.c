// In The Name Of God
// ========================================
// * File Name : main.c
// 
// * Creation Date : 06-12-2014
//
// * Last Modified : Sat 20 Dec 2014 11:33:49 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"

int main(int argc, char* argv[]){
	printf("TFAT version 01, Copyright (C) 2015 Parham Alvani(parham.alvani@gmail.com)\n");
	printf("TFAT comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions; type `show c' for details.\n");
	printf("\n");

	char* command;
	while(true){
		command = readline("TFAT> ");
		if(command && *command){
			add_history(command);
		}
		command_dispatcher(command);
		free(command);
	}
}
