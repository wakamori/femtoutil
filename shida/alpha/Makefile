CFLAGS=-g -O2
a : main.o token.o tree.o
	gcc $(CFLAGS) -o  a main.o token.o tree.o -lm -O2

main.o : main.c main.h
	gcc $(CFLAGS) -c main.c

token.o : token.c token.h 
	gcc $(CFLAGS) -c token.c
	
tree.o : tree.c tree.h
	gcc $(CFLAGS) -c tree.c

.PHONY:clean
clean:
	-rm *.o
