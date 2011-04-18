CFLAGS=-g -O2
a : main.o token.o tree.o stack_data.o
	gcc $(CFLAGS) -o  a main.o token.o tree.o stack_data.o -lm -O2

main.o : main.c main.h
	gcc $(CFLAGS) -c main.c

token.o : token.c token.h 
	gcc $(CFLAGS) -c token.c

tree.o : tree.c tree.h
	gcc $(CFLAGS) -c tree.c

stack_data.o : stack_data.c stack_data.h
	gcc $(CFLAGS) -c stack_data.c

.PHONY:clean
clean:
	-rm *.o
