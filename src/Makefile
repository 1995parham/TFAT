# In The Name Of God
# ========================================
# [] File Name : Makefile
#
# [] Creation Date : 10-01-2015
#
# [] Created By : Parham Alvani (parham.alvani@gmail.com)
# =======================================

P=FAT.out
OBJ=main.o FAT.o user.o common.o command.o fs.o lfn.o cd.o label.o io.o \
    FAT16.o FAT32.o
LIB=-ltermcap
SLIB=libreadline.a libhistory.a
CFLAGS=-g -Wall -Iinclude

$(P): $(OBJ)
	$(CC) $(CFLAGS) -o $(P) $(OBJ) $(SLIB) $(LIB)

main.o: command.h cd.h label.h
user.o: user.h FAT.h common.h fs.h lfn.h cd.h label.h
common.o: common.h
command.o: command.h user.h common.h cd.h label.h
FAT.o: FAT.h FAT16.h FAT32.h common.h
FAT16.o: FAT.h FAT16.h common.h
FAT32.o: FAT.h FAT32.h common.h
fs.o: fs.h FAT.h io.h common.h
lfn.o: lfn.h FAT.h common.h
cd.o: cd.h
label.o: label.h
io.o: io.h

.PHONY: clean
clean:
	rm $(OBJ) FAT.out
