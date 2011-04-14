CC = cc
TARGET = lisp
OBJS = lisp.o memory.o parser.o eval.o

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

.SUFFIXES: .c.o

.c.o:
	$(CC) -c $*.c

clean:
	rm -f $(TARGET) $(OBJS)

