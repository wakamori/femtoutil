
include nativelink;


@Native class Clib;

@Native Clib Clib.new(String libname);

@Native class Dglue; /* func information */
@Native Dglue Clib.genGlue(Dglue _);
@Native var Dglue.getFunc(String sym, Class _, Func _);
