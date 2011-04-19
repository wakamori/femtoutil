CFLAGS=-g -O2
CC=gcc
TARGET=a
OBJS=main.o token.o data_structure.o
LDLIBS=-lm

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o : main.c main.h
	$(CC) $(CFLAGS) -c main.c
token.o : token.c token.h 
	$(CC) $(CFLAGS) -c token.c
data_structure.o : data_structure.c data_structure.h 
	$(CC) $(CFLAGS) -c data_structure.c

.PHONY:clean
clean:
	-$(RM) $(TARGET) $(OBJS)
