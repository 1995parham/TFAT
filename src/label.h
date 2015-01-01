/*
 * In The Name Of God
 * ========================================
 * [] File Name : label.h
 *
 * [] Creation Date : 31-12-2014
 *
 * [] Last Modified : Thu 01 Jan 2015 04:59:50 AM IRST
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

void is_label_on_BS();

#endif
