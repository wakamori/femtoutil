CFLAGS= -O2
CC=gcc
TARGET=a
OBJS=main.o eval.o parser.o generator.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c main.h
	$(CC) $(CFLAGS) -c main.c

eval.o : eval.c
	$(CC) $(CFLAGS) -c eval.c

parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

generator.o : generator.c generator.h
	$(CC) $(CFLAGS) -c generator.c

.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
