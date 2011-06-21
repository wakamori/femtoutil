/*
 * Clib
 *  C library wrapper generater using libffi
 */
include nativelink;

@Native class Clib;
@Native Clib Clib.new(String libname);
@Native class Process;
@Native Process Process.new(String path);

//@Native class Openapi;
//@Native Openapi Openapi.new(String url);

@Native class Glue;
@Native @Throwable var Glue.getFunc(String symbol, Class _, Func _);

@Native Glue Clib.genGlue(Glue _);
@Native Glue Process.genGlue(Glue _);
//@Native Glue Openapi.genGlue(Glue _);
