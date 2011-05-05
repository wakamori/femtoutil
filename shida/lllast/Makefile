CFLAGS=  -O2 -g
LDFLAGS=`llvm-config --libs core jit native --cxxflags --ldflags`

CC=g++
TARGET=a
OBJS=main.o parser.o generator.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o : main.cpp 
	$(CC) $(CFLAGS) -c main.cpp $(LDFLAGS)

parser.o : parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp $(LDFLAGS)

generator.o : generator.cpp
	$(CC) $(CFLAGS) -c generator.cpp $(LDFLAGS)


.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
