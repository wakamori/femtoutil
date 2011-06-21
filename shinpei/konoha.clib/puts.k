using konoha.clib.*;

Clib c = new Clib("/usr/lib/libc.dylib");
Glue d = c.genGlue();

Func<String=>Int> puts = d.getFunc("puts"); 
print puts("hello world");

