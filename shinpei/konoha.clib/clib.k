
include nativelink;

@Native class Clib;
@Native Clib Clib.new(String libname);

import "dglue.k";

@Native Glue Clib.genGlue(Glue _);
//@Native var Dglue.getFunc(String sym, Class _, Func _);
