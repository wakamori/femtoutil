using konoha.clib.*;

Clib libc = new Clib("/usr/lib/libc.dylib");
Glue g = libc.genGlue();

Clib.makeClass("Dir");
//s = new Structure([("x", Structure.FFI_POINTER)]);

s = new Structure([["x", Structure.FFI_POINTER]]);

/*
Clib.makeClass(
	"Dir",
	new Structure("dir", Structure.FFI_POINTER) 
)
*/

Func<String=>Dir> opendir = g.getFunc("opendir");

d = opendir("/etc/");
print d;

Func<Dir=>Int> telldir = g.getFunc("telldir");

print telldir(d);


