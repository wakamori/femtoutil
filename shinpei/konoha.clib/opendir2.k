using konoha.clib.*;

Clib libc = new Clib("/usr/lib/libc.dylib");
Glue g = libc.genGlue();

Clib.makeClass("Dir");

Func<String=>Dir> opendir = g.getFunc("opendir");

d = opendir("/etc/");
print d;

Func<Dir=>Int> telldir = g.getFunc("telldir");

print telldir(d);


