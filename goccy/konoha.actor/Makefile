CC = gcc
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 -fPIC -DK_USING_THREAD
ARCH = #-arch i386 -arch x86_64
LDLIBS = -lkonoha1 -lpthread -lmemcached
version = 1.0
LIBVER = -current_version 1.0.0 -compatibility_version $(version)
pkgname = actor
library = "$(pkgname).dylib"

.PHONY: all
all: $(library)

objs = actor.o

actor.o: actor.c
	$(CC) $(CFLAGS) $(ARCH) -D_SETUP -o $@ -c $^

$(library): $(objs)
	$(CC) $(ARCH) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library)
