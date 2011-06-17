CC = g++
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 -fPIC -I/usr/local/include/ -I/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers
LDLIBS = -L/usr/local/lib/ -lkonoha1 -lpthread -lmemcached -lmsgpack -framework JavaVM
version = 1.0
LIBVER = -current_version 1.0.0 -compatibility_version $(version)
pkgname = actor
library = "$(pkgname).dylib"

.PHONY: all
all: $(library)

objs = build/actor.o build/stream.o

build/actor.o: src/actor.cpp
	$(CC) $(CFLAGS) -D_SETUP -o $@ -c $^

build/stream.o: src/stream.cpp
	$(CC) $(CFLAGS) -D_SETUP -o $@ -c $^

$(library): $(objs)
	$(CC) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library)
