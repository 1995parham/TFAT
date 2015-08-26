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
cmake_minimum_required (VERSION 2.6)
project (TFAT)

# Set porject imformation
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

subdirs (src)

# The version number
set (RBP_MAJOR_VERSION 1)
set (RBP_MINOR_VERSION 1)

# Add personal c flags
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -g -std=gnu11 ")
