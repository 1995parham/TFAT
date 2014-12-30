/*
 * In The Name Of God
 * ========================================
 * [] File Name : label.h
 *
 * [] Creation Date : 31-12-2014
 *
 * [] Last Modified : Wed 31 Dec 2014 12:39:36 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef LABEL_H
#define LABEL_H

/*
 * This string containing current
 * disk label.
 * please change it with following
 * command and DO NOT change it manually
*/
extern char *label;

void change_label(const char *new_label);

#endif
