# Makefile

TARGET = lisugimoto
OBJS = \
	lisugimoto.o\
	read.o\
	eval.o\
	print.o
CC = gcc
CFLAGS = -g3 -Wall -O0
LDLIBS = -lreadline

.PHONY: all
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $^

lisugimoto.o: lisugimoto.c
	$(CC) $(CFLAGS) -c $<

read.o: read.c
	$(CC) $(CFLAGS) -c $<

eval.o: eval.c
	$(CC) $(CFLAGS) -c $<

print.o: print.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
