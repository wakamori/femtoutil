include nativelink;

// local glue
@Native class Process;

@Native Process Process.new(String path);

//@Native class Lglue;
@Native Lglue Process.genGlue(Lglue _);
@Native var Lglue.getFunc(Class _, Func _);



