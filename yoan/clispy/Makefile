# for clispy

CC= gcc
CFLAGS= -W -Wall -g3 -O0 
INCLUDE= include/*.h
SRC= src/*.c
OBJ= main.o eval.o parse.o cell_checker.o

TARGET= clispy
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
main.o: ./src/main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
eval.o: ./src/eval.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
parse.o: ./src/parse.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
cell_checker.o: ./src/cell_checker.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET) 
