/*
 * In The Name Of God
 * ========================================
 * [] File Name : label.h
 *
 * [] Creation Date : 02-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
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
