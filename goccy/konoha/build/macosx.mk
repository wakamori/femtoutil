# Makefile for macosx
#

CC ?= MACOSX_DEPLOYMENT_TARGET=10.5 /usr/bin/gcc
CFLAGS ?= -O0 -g3 -Wall -fPIC -I./include
#CFLAGS ?= -g3 -O0 -Wall -fPIC -I./include -DK_USING_DEBUG

ARCH ?= -arch i386 -arch x86_64
LDLIBS ?= -lpcre -liconv -lsqlite3 -lpthread
STRIP = strip

konoha = konoha1
version = 1.0
LIBVER = -current_version 1.0.0 -compatibility_version $(version)
PREFIX = /usr/local
dir    = build

packages = \
	$(dir)/lib$(konoha).dylib \
	$(dir)/i.dylib \
	$(dir)/math.dylib \
	$(dir)/socket.dylib \
	$(dir)/posix.dylib \

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
	$(dir)/ffi.o \
	$(dir)/stack.o\
	$(dir)/stream.o \
	$(dir)/string.o \
	$(dir)/struct.o \
	$(dir)/system.o \
	$(dir)/thread.o\
	$(dir)/mt19937ar.o \

svnversion_exists := $(shell which svnversion)
define compile_with_revision
	$(CC) -DK_REVISION=$(shell $(svnversion_exists) -n ./ | sed -e 's/[MSP]//')
endef

define compile
	$(CC) -DK_REVISION=$(shell $(svnversion_exists) -n ./)
endef

MYCC =$(if $(svnversion_exists) ,$(compile_with_revision), $(compile))

.PHONY: all
all: $(dir)/$(konoha) $(packages)

$(dir)/$(konoha) : src/konoha.c $(dir)/lib$(konoha).dylib $(dir)/lib$(konoha).a
	$(MYCC) $(CFLAGS) $(ARCH) -o $@ src/konoha.c -L./$(dir) -l$(konoha) 

$(dir)/lib$(konoha).dylib : $(objs)
	$(MYCC) $(CFLAGS) $(ARCH) -dynamiclib $(LIBVER) -install_name /usr/local/lib/lib$(konoha).dylib -o $@ $^ $(LDLIBS)

$(dir)/lib$(konoha).a: $(objs)
	$(AR) rv $@ $^

## object files

$(dir)/asm.o : src/lang/asm.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/term.o : src/lang/term.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@	

$(dir)/typing.o : src/lang/typing.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/script.o : src/lang/script.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/vmcodelibs.o : src/lang/vmcodelibs.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/message.o : src/lang/message.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/array.o : src/main/array.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/bytes.o : src/main/bytes.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/class.o : src/main/class.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/closure.o : src/main/closure.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/context.o: src/main/context.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/evidence.o : src/main/evidence.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/exports.o : src/main/exports.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/map.o : src/main/map.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/memory.o: src/main/memory.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/number.o : src/main/number.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/os.o: src/main/os.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/query.o : src/main/query.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/runtime.o : src/main/runtime.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/security.o : src/main/security.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/semantics.o: src/main/semantics.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/ffi.o : src/main/ffi.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/stack.o: src/main/stack.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/stream.o : src/main/stream.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/string.o : src/main/string.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/struct.o : src/main/struct.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/system.o : src/main/system.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/thread.o: src/main/thread.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/mt19937ar.o : src/ext/mt19937ar.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

$(dir)/konoha.o : src/konoha.c
	$(MYCC) $(CFLAGS) $(ARCH) -c $^ -o $@

## konoha.i interactive mode

LDLIBS_libi = 
objs_i = $(dir)/i.o\
	
$(dir)/i.dylib: $(objs_i)
	$(MYCC) $(CFLAGS) $(ARCH) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS_libi)
	
$(dir)/i.o : package/konoha.i/i.c
	$(MYCC) $(CFLAGS) $(ARCH) -D_SETUP -c $^ -o $@

## math
LDLIBS_libmath = -lm
objs_math = $(dir)/math.o
	
$(dir)/math.dylib: $(objs_math)
	$(MYCC) $(CFLAGS) $(ARCH) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS_libmath)
	
$(dir)/math.o : package/konoha.math/math.c
	$(MYCC) $(CFLAGS) $(ARCH) -D_SETUP -c $^ -o $@

## socket
LDLIBS_libsocket = 
objs_socket = $(dir)/socket.o
	
$(dir)/socket.dylib: $(objs_socket)
	$(MYCC) $(CFLAGS) $(ARCH) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS_libsocket)

$(dir)/socket.o : package/konoha.socket/socket_posix.c
	$(MYCC) $(CFLAGS) $(ARCH) -D_SETUP -c $^ -o $@

## posix
LDLIBS_libposix = 
objs_posix = $(dir)/posix.o
	
$(dir)/posix.dylib: $(objs_posix)
	$(MYCC) $(CFLAGS) $(ARCH) -dynamiclib $(LIBVER) -o $@ $^ $(LDLIBS_libposix)

$(dir)/posix.o : package/konoha.posix/posix.c
	$(MYCC) $(CFLAGS) $(ARCH) -D_SETUP -c $^ -o $@

## install
.PHONY: install
install:
	bash $(dir)/uninstall.sh $(konoha) $(PREFIX)
	bash $(dir)/install.sh $(konoha) $(PREFIX)
	bash $(dir)/pkginstall.sh math $(PREFIX)/konoha/package/$(version) konoha.math
	bash $(dir)/pkginstall.sh i $(PREFIX)/konoha/package/$(version) konoha.i
	bash $(dir)/pkginstall.sh socket $(PREFIX)/konoha/package/$(version) konoha.socket
	bash $(dir)/pkginstall.sh posix $(PREFIX)/konoha/package/$(version) konoha.posix

## uninstall
.PHONY: uninstall
uninstall:
	bash $(dir)/uninstall.sh $(konoha) $(PREFIX)

## clean
.PHONY: clean
clean:
	$(RM) -rf $(dir)/*.dylib $(dir)/*.o $(dir)/$(konoha) $(dir)/lib$(konoha).a

