CC = g++
TARGET = lllvm
OBJS = compile.o memory.o parser.o lisp.o
CFLAGS = `llvm-config --cppflags --ldflags --libs core jit native` -O2

$(TARGET) : $(OBJS)
	$(CC) $) $(OBJS) $(CFLAGS) -o $(TARGET)
.SUFFIXES: .cpp.o

.cpp.o:
	$(CC) -c $*.cpp $(CFLAGS) 
clean:
	rm -f $(TARGET) $(OBJS)

