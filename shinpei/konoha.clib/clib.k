
include nativelink;


@Native class Clib;
@Native class Cfunc;

@Native Clib Clib.new(String libname);
@Native Cfunc Clib.getSymbol(String sym, Class retT, Array<Class> argT, NameSpace _, Cfunc _);
@Native Float Cfunc.invoke(Float d);
