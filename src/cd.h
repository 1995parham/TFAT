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
