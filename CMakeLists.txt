cmake_minimum_required(VERSION 3.2)
project(TFAT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/include/readline/ansi_stdlib.h
    src/include/readline/chardefs.h
    src/include/readline/colors.h
    src/include/readline/config.h
    src/include/readline/histlib.h
    src/include/readline/history.h
    src/include/readline/keymaps.h
    src/include/readline/parse-colors.h
    src/include/readline/posixdir.h
    src/include/readline/posixjmp.h
    src/include/readline/posixselect.h
    src/include/readline/posixstat.h
    src/include/readline/readline.h
    src/include/readline/rlconf.h
    src/include/readline/rldefs.h
    src/include/readline/rlmbutil.h
    src/include/readline/rlprivate.h
    src/include/readline/rlshell.h
    src/include/readline/rlstdc.h
    src/include/readline/rltty.h
    src/include/readline/rltypedefs.h
    src/include/readline/rlwinsize.h
    src/include/readline/tcap.h
    src/include/readline/tilde.h
    src/include/readline/xmalloc.h
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