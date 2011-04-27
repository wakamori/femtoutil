CC = gcc
CFLAGS= -O2 -g0 -Wall
LDLIBS= -lreadline -lm

target = liso
objs = main.o eval.o parse.o token.o

.PHONY: all
$(target): $(objs)
	$(CC) -o $@ $^ $(LDLIBS)
eval.o : eval.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

parse.o : parse.c
	$(CC) $(CFLAGS) -o $@ -c $^

token.o : token.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	$(RM) $(target) $(objs)
