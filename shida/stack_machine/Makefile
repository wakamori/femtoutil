CFLAGS=-g -O2
a : main.o token.o data_structure.o 
	gcc $(CFLAGS) -o  a main.o token.o data_structure.o -lm -O2

main.o : main.c main.h
	gcc $(CFLAGS) -c main.c
token.o : token.c token.h 
	gcc $(CFLAGS) -c token.c
data_structure.o : data_structure.c data_structure.h 
	gcc $(CFLAGS) -c data_structure.c

.PHONY:clean
clean:
	-rm *.o
