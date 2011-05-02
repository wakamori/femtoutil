CFLAGS= -O2 -W -Wall
CC=g++
TARGET=a
OBJS=main.o eval.o parser.o generator.o hash.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

eval.o : eval.c
	$(CC) $(CFLAGS) -c eval.c

parser.o : parser.c
	$(CC) $(CFLAGS) -c parser.c

generator.o : generator.c
	$(CC) $(CFLAGS) -c generator.c

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
