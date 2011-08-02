using konoha.clib.*;

Clib c = new Clib("/usr/lib/libc.dylib");
Glue d = c.genGlue();

Func<int=>String> puts = d.getFunc("puts"); 
print puts(3);
//print puts("hello world");

