// In The Name Of God
// ========================================
// * File Name : common.c
// 
// * Creation Date : 05-12-2014
//
// * Last Modified : Wed 17 Dec 2014 01:33:12 AM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void die(const char *fmt, ...){
	char buf[MAX_BUFF];
	
	va_list args; 
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	perror(buf);
	exit(EXIT_FAILURE);
}
