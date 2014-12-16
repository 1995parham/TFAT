OBJ=main.o FAT.o
LIB=libreadline.a libhistory.a
CFLAGS=-gstabs+ -Wall

FAT: $(OBJ)
	gcc $(CFLAGS) -o FAT.out $(OBJ) $(LIB)
main.o: main.c FAT.h
	gcc $(CFLAGS) -c main.c
FAT.o: FAT.c FAT.h
	gcc $(CFLAGS) -c FAT.c
.PHONY: clean
clean:
	rm $(OBJ) FAT.out
