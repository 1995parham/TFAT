OBJ=main.o FAT.o user.o common.o command.o fs.o lfn.o cd.o label.o io.o \
    FAT16.o FAT32.o
LIB=-ltermcap
SLIB=libreadline.a libhistory.a
CFLAGS=-gstabs+ -Wall -Iinclude

FAT: $(OBJ)
	gcc $(CFLAGS) -o FAT.out $(OBJ) $(SLIB) $(LIB)
main.o: main.c command.h cd.h label.h
	gcc $(CFLAGS) -c main.c
user.o: user.c user.h FAT.h common.h fs.h lfn.h cd.h label.h
	gcc $(CFLAGS) -c user.c
common.o: common.h common.c
	gcc $(CFLAGS) -c common.c
command.o: command.c command.h user.h common.h \
	cd.h label.h
	gcc $(CFLAGS) -c command.c
FAT.o: FAT.c FAT.h FAT16.h FAT32.h common.h
	gcc $(CFLAGS) -c FAT.c
FAT16.o: FAT16.c FAT.h FAT16.h common.h
	gcc $(CFLAGS) -c FAT16.c
FAT32.o: FAT32.c FAT.h FAT32.h common.h
	gcc $(CFLAGS) -c FAT32.c
fs.o: fs.h fs.c FAT.h io.h common.h
	gcc $(CFLAGS) -c fs.c
lfn.o: lfn.c lfn.h FAT.h common.h
	gcc $(CFLAGS) -c lfn.c
cd.o: cd.h cd.c
	gcc $(CFLAGS) -c cd.c
label.o: label.h label.c
	gcc $(CFLAGS) -c label.c
io.o: io.h io.c
	gcc $(CFLAGS) -c io.c
.PHONY: clean
clean:
	rm $(OBJ) FAT.out
