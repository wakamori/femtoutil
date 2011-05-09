# Makefile

TARGET = lisugimoto
OBJS = \
	lisugimoto.o\
	read.o\
	eval.o\
	print.o\
	vhash.o\
	fhash.o\
	stack.o\
	error.o
CC = gcc
CFLAGS =  -g3 -Wall 
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

vhash.o: vhash.c
	$(CC) $(CFLAGS) -c $<

fhash.o: fhash.c
	$(CC) $(CFLAGS) -c $<

stack.o: stack.c
	$(CC) $(CFLAGS) -c $<

error.o: error.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
