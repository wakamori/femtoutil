using konoha.clib.*;

Clib c = new Clib("/usr/lib/libc.dylib");
Glue d = c.genGlue();

Func<String=>Int> strlen = d.getFunc("strlen"); 
s = "hello world";
i = strlen(s);
print i;

