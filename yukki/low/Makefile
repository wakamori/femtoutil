CC = cc
TARGET = lisp
OBJS = lisp.o parser.o eval.o list.o

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -O2

.SUFFIXES: .c.o

.c.o:
	$(CC) -c $*.c

clean:
	rm -f $(TARGET) $(OBJS)

