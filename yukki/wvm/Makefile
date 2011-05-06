CC = gcc
TARGET = lisp
OBJS = lisp.o parser.o eval.o memory.o inst.o func.o
#OBJS = inst.o fibtest.o
CFLAGS ?= -O2 -g3
#CFLAGS ?= -g3 -O0 -pg

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) 
.SUFFIXES: .c.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c 
clean:
	rm -f $(TARGET) $(OBJS)

