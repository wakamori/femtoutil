CC = gcc
CFLAGS= -O0 -g3 -Wall 
LDLIBS= -lreadline -lm

target = lispei
objs = lisp.o\
#       hoge.o

.PHONY: all
$(target): $(objs)
	$(CC) -o $@ $< $(LDLIBS) $(CFLAGS)

#hoge.o : hoge.c
#	$(CC) -c $< $(CFLAGS)

lisp.o: lisp.c
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	$(RM) $(target) $(objs)
