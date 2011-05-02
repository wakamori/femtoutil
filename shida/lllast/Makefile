CFLAGS= -O2 -W -Wall
CC=g++
TARGET=a
OBJS=main.o eval.o parser.o generator.o hash.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

eval.o : eval.cpp
	$(CC) $(CFLAGS) -c eval.cpp

parser.o : parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp

generator.o : generator.cpp
	$(CC) $(CFLAGS) -c generator.cpp

hash.o : hash.cpp
	$(CC) $(CFLAGS) -c hash.cpp

.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
