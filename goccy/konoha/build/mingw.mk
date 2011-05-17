# Makefile for mingw
#

CC = gcc
CFLAGS = -g3 -O0 -Wall -I./include -DK_USING_WINTHREAD_ -DK_USING_WINDOWS -DK_USING_MINGW
LDLIBS =  -lpthread -lws2_32
STRIP = strip

konoha = konoha1
version = 1.0
PREFIX = $(DESTDIR)/usr/local
dir    = build
pkgdir = $(dir)/package/$(version)

packages = \
	$(dir)/lib$(konoha).dll \
	$(pkgdir)/konoha.i/i.dll \
	$(pkgdir)/konoha.math/math.dll \
#	$(pkgdir)/konoha.posix/posix.dll \

objs = \
	$(dir)/asm.o \
	$(dir)/term.o \
	$(dir)/typing.o\
	$(dir)/script.o\
	$(dir)/vmcodelibs.o\
	$(dir)/message.o\
	$(dir)/array.o \
	$(dir)/bytes.o \
	$(dir)/class.o\
	$(dir)/closure.o \
	$(dir)/context.o\
	$(dir)/evidence.o \
	$(dir)/exports.o \
	$(dir)/map.o \
	$(dir)/memory.o\
	$(dir)/number.o \
	$(dir)/os.o\
	$(dir)/query.o \
	$(dir)/runtime.o \
	$(dir)/security.o \
	$(dir)/semantics.o\
	$(dir)/stack.o\
	$(dir)/stream.o \
	$(dir)/string.o \
	$(dir)/struct.o \
	$(dir)/system.o \
	$(dir)/ffi.o \
	$(dir)/thread.o\
	$(dir)/mt19937ar.o \

.PHONY: all
all: $(dir)/$(konoha) $(packages)

$(dir)/$(konoha) : src/konoha.c $(dir)/lib$(konoha).dll $(dir)/konoha.res
	$(CC) $(CFLAGS) -o $@ src/konoha.c -L./$(dir) -l$(konoha) $(LDLIBS) $(dir)/konoha.res
#	$(STRIP) $@

$(dir)/lib$(konoha).dll: $(objs)
	$(CC) $(CFLAGS) -L./$(dir) -shared -o $@ $^ $(LDLIBS)

$(dir)/konoha.res: $(dir)/konoha.rc
	windres $(dir)/konoha.rc -O coff -o $(dir)/konoha.res

## object files

$(dir)/asm.o : src/lang/asm.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/term.o : src/lang/term.c
	$(CC) $(CFLAGS) -c $^ -o $@	

$(dir)/typing.o : src/lang/typing.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/script.o : src/lang/script.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/vmcodelibs.o : src/lang/vmcodelibs.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/message.o : src/lang/message.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/array.o : src/main/array.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/bytes.o : src/main/bytes.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/class.o : src/main/class.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/closure.o : src/main/closure.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/context.o: src/main/context.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/evidence.o : src/main/evidence.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/exports.o : src/main/exports.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/map.o : src/main/map.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/memory.o: src/main/memory.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/number.o : src/main/number.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/os.o: src/main/os.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/query.o : src/main/query.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/ffi.o : src/main/ffi.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/runtime.o : src/main/runtime.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/security.o : src/main/security.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/semantics.o: src/main/semantics.c
	$(CC) $(CFLAGS) -c $^ -o $@

#$(dir)/setjmp.o : src/main/setjmp.c
#	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/stack.o: src/main/stack.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/stream.o : src/main/stream.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/string.o : src/main/string.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/struct.o : src/main/struct.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/system.o : src/main/system.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/thread.o: src/main/thread.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(dir)/mt19937ar.o : src/ext/mt19937ar.c
	$(CC) $(CFLAGS) -c $^ -o $@

## konoha.i interactive mode

LDLIBS_libi = 
objs_i = $(dir)/i.o\

$(pkgdir)/konoha.i/i.dll: $(objs_i)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDLIBS_libi)

$(dir)/i.o : package/konoha.i/i.c
	$(CC) $(CFLAGS) -D_SETUP -c $^ -o $@

## math
LDLIBS_libmath = -lm
obj_math = $(dir)/math.o\

$(pkgdir)/konoha.math/math.dll: $(obj_math)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDLIBS_libmath)

$(dir)/math.o : package/konoha.math/math.c
	$(CC) $(CFLAGS) -D_SETUP -c $^ -o $@

## posix
LDLIBS_libposix = 
objs_posix = $(dir)/posix.o\

$(pkgdir)/konoha.posix/posix.dll: $(objs_posix)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDLIBS_libposix)

$(dir)/posix.o : package/konoha.posix/posix.c
	$(CC) $(CFLAGS) -D_SETUP -c $^ -o $@

## install
.PHONY: install
install:
	bash $(dir)/uninstall.sh $(konoha) $(PREFIX)
	bash $(dir)/install.sh $(konoha) $(PREFIX)
	bash $(dir)/pkginstall.sh i $(PREFIX)/konoha/package/$(version) konoha.i
	bash $(dir)/pkginstall.sh math $(PREFIX)/konoha/package/$(version) konoha.math
	bash $(dir)/pkginstall.sh posix $(PREFIX)/konoha/package/$(version) konoha.posix

## uninstall
.PHONY: uninstall
uninstall:
	bash $(dir)/uninstall.sh $(konoha) $(PREFIX)

## clean
.PHONY: clean
clean:
	$(RM) -rf $(dir)/*.dll $(dir)/*.o $(dir)/*.res $(dir)/$(konoha) $(pkgdir)/konoha.i/*.dll $(pkgdir)/konoha.math/*.dll $(pkgdir)/konoha.posix/*.dll
