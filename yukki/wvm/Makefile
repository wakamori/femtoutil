CC = cc
TARGET = lisp
OBJS = lisp.o parser.o eval.o memory.o inst.o
#OBJS = inst.o fibtest.o
CFLAGS ?= -O2
#CFLAGS ?= -g3 -O0 -pg

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

.SUFFIXES: .c.o

.c.o:
	$(CC) -c $*.c $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

