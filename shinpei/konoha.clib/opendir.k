using konoha.clib.*;

class Dir;
Clib libc = new Clib("/usr/lib/libc.dylib");
Glue g = libc.genGlue();

Func<String=>Dir> opendir = g.getFunc("opendir");

d = opendir("/etc/");
print d;

Func<Dir=>Int> telldir = g.getFunc("telldir");
print telldir(d);
