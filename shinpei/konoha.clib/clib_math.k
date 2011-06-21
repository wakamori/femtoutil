using konoha.clib.*;

Clib c = new Clib("/usr/lib/libm.dylib");
Glue d = c.genGlue();

Func<Float=>Float> sin = d.getFunc("sin"); /* typing */
print sin(1.5);

Func<Float=>Float> cos = d.getFunc("cos");
print cos(1.0);
