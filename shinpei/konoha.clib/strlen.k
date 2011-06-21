using konoha.clib.*;

Clib c = new Clib("/usr/lib/libc.dylib");
print c;
Glue g = c.genGlue();
print g;
@Throwable Func<String=>Int> strlen = g.getFunc("strlen"); 
s = "hello world";
i = strlen(s);
print i;

