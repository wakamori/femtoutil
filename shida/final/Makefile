CFLAGS= -g -O2
CC=gcc
TARGET=a
OBJS=main.o eval.o parser.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

main.o : main.c main.h
	$(CC) $(CFLAGS) -c main.c

eval.o : eval.c eval.h
	$(CC) $(CFLAGS) -c eval.c

parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
