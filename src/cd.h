/*
 * In The Name Of God
 * ========================================
 * [] File Name : cd.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef CD_H
#define CD_H

/*
 * This string containing current
 * directory and we use it with our cd
 * command.
 * cd A  ==> current_path += A
 * cd /A ==> current_path = A
 * please change it with following command
 * and DO NOT change it manually.
*/
extern char *current_path;


void change_current_path(const char *new_path);

void concat_current_path(const char *new_path);

#endif
