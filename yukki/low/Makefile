CC = cc
TARGET = lisp
OBJS = lisp.o parser.o eval.o list.o memory.o
#CFLAGS ?= -O2
CFLAGS ?= -g3 -O0 -pg

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -pg

.SUFFIXES: .c.o

.c.o:
	$(CC) -c $*.c $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS) *~

