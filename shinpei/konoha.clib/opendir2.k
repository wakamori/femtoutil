using konoha.clib.*;

Clib libc = new Clib("/usr/lib/libc.dylib");
Glue g = libc.genGlue();

//Clib.defineClass("Dir", closedir);

Clib.makeClass("Dir");

Func<String=>var> opendir = g.getFunc("opendir");

d = opendir("/etc/");
print d;

Func<var=>Int> telldir = g.getFunc("telldir");

print telldir(d);


