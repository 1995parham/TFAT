OBJ=main.o FAT.o user.o common.o command.o fs.o
LIB=-ltermcap
SLIB=libreadline.a libhistory.a
CFLAGS=-gstabs+ -Wall -Iinclude

FAT: $(OBJ)
	gcc $(CFLAGS) -o FAT.out $(OBJ) $(SLIB) $(LIB)
main.o: main.c command.h
	gcc $(CFLAGS) -c main.c
user.o: user.c user.h FAT.h common.h fs.h
	gcc $(CFLAGS) -c user.c
common.o: common.h common.c
	gcc $(CFLAGS) -c common.c
command.o: command.c command.h user.h common.h
	gcc $(CFLAGS) -c command.c
FAT.o: FAT.c FAT.h
	gcc $(CFLAGS) -c FAT.c
fs.o: fs.h fs.c
	gcc $(CFLAGS) -c fs.c
.PHONY: clean
clean:
	rm $(OBJ) FAT.out
