#
#  TFAT, Fat parser and cli
#  Copyright (C) 2015  Parham Alvani (parham.alvani@gmail.com)
#  Copyright (C) 2015  Elahe Jalalpour (el.jalalpour@gmail.com)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
add_executable(TFAT.out cd.c command.c common.c FAT.c FAT16.c FAT32.c fs.c io.c /
        label.c lfn.c main.c user.c)

target_link_libraries(TFAT.out readline)
