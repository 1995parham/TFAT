cmake_minimum_required(VERSION 3.2)
project(TFAT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/cd.c
    src/cd.h
    src/command.c
    src/command.h
    src/common.c
    src/common.h
    src/FAT.c
    src/FAT.h
    src/FAT16.c
    src/FAT16.h
    src/FAT32.c
    src/FAT32.h
    src/fs.c
    src/fs.h
    src/io.c
    src/io.h
    src/label.c
    src/label.h
    src/lfn.c
    src/lfn.h
    src/main.c
    src/user.c
    src/user.h)

add_executable(TFAT ${SOURCE_FILES})