CFLAGS= -O2
CC=g++
TARGET=a
OBJS=main.o parser.o ast.o lexer.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

lexer.o : lexer.cpp lexer.h
	$(CC) $(CFLAGS) -c lexer.cpp

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o : parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

ast.o : ast.cpp ast.h
	$(CC) $(CFLAGS) -c ast.cpp


.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
