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
#ifndef IO_H
#define IO_H

/*
 * Returns a non-zero integer if size bytes starting at
 * pos can be read without errors. Otherwise, it returns zero.
*/
int fs_test(int fd, off_t pos, int size);

#endif
