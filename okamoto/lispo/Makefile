TARGET=lispo
CC=gcc
OBJS=\
     main.o\
	 make_cons.o\
	 search.o\
	 other_func.o\
	 eval.o\
	 GC.o
CFLAGS=-g3 -O2
LDLIBS=-lreadline -lcurses

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $^

main.o: main.c
	$(CC) $(CFLAGS) -c $^

main.o: lispo.h

make_cons.o: make_cons.c
	$(CC) $(CFLAGS) -c $^

make_cons.o: lispo.h

search.o: search.c
	$(CC) $(CFLAGS) -c $^

search.o: lispo.h

other_func.o: other_func.c
	$(CC) $(CFLAGS) -c $^

other_func.o: lispo.h

eval.o: eval.c
	$(CC) $(CFLAGS) -c $^

eval.o: lispo.h

GC.o: GC.c
	$(CC) $(CFLAGS) -c $^

GC.o: lispo.h

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) *~ *.gch gmon.*
