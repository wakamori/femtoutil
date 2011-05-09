CC = gcc
CFLAGS= -O2 -Wall
#CFLAGS= -g3 -Wall
#CFLAGS= -pg
LDLIBS= -lreadline -lm

target = liso
objs = main.o eval.o parse.o token2.o

.PHONY: all
$(target): $(objs)
	$(CC) -o $@ $^ $(LDLIBS)
eval.o : eval.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

parse.o : parse.c
	$(CC) $(CFLAGS) -o $@ -c $^

token2.o : token2.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	$(RM) $(target) $(objs)
