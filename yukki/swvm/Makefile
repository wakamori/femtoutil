CC = g++
TARGET = lisp
OBJS = exec.o codegen.o lisp.o parser.o memory.o func.o
CFLAGS ?= -O2 -g3 -Wall

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)  -lreadline
.SUFFIXES: .c.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c 
clean:
	rm -f $(TARGET) $(OBJS)

