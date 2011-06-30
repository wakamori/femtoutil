/*
 * Dependable FFI
 *  CLib: library wrapper generator using libffi
 *  Process : process wrapper generator.
 */

include nativelink;

@Native class Clib;
@Native Clib Clib.new(String libname, Clib _);
@Native class Process;
@Native Process Process.new(String path, Process _);

@Native class Webservice;
@Native Webservice Webservice.new(String url);

@Native class Glue;
@Native @Throwable var Glue.getFunc(String symbol, Class _, Func _);

@Native Glue Clib.genGlue(Glue _);
@Native Glue Process.genGlue(Glue _);
@Native Glue Webservice.genGlue(Glue _);

@Native void Clib.makeClass(String classname NameSpace _);

