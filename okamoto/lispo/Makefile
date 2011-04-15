TARGET1=lispo
TARGET2=exe
CC=gcc
OBJS=\
     lispo.o
CFLAGS=-g3 -O0
LDLIBS=-lreadline -lcurses

.PHONY: all
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS)
	$(CC) $(LDLIBS) -o $(TARGET1) $^

$(TARGET2): $(OBJS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET2) $^

lispo.o: lispo.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean
clean:
	$(RM) $(TARGET1) $(TARGET2) $(OBJS)
