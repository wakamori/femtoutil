using konoha.clib.*;

mwlib = new Clib("/usr/local/lib/libMagickWand.dylib");
Structure c_mw = new Structure([
	["self", Structure.FFI_POINTER]
	]);
Clib.defineClass("Magickwand", c_mw);

Glue mwg = mwlib.genGlue();

void Magickwand.coreTerminus() {
	Func<Magickwand=>void> f = mwg.getFunc("coreTerminus");
	f(this);
}




