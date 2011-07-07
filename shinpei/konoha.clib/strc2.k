using konoha.clib.*;

c_test = new Structure([["test", Structure.FFI_UINT]]);
Clib.defineClass("Test", c_test);

struct {
	int nval;
	const char *sval;
} nkt_t;

c_nkt_t = new Strcutre([["nval", Strcuture.FFI_UINT], ["sval", Structure.FFI_STRING]]);
Clib.defineClass("NKT", c_nkt_t);


include ffi:libc;
@Native @RAII class Dir using free:closedir;

void f(nkt_t, nkt_t)

@Native Dir opendir(String path, NameSpace _, Dir _)
	using ffi:opendir(path:(1,2)=>Dir:3)

@Native void NKT.f(NKT v) 
  using ffi:f(word2:0,word2:1);

@Native float Math.sin(float v) 
   using ffi:sin(double:1=>double);



==> libffi
    AOT
    LLVM generate

sfp[1].o
sfp[1].ndata

double ret = sin((double)sfp[1].fvalue)
RETURNf_(ret)

sfp[1].ivalue
_1i, _2f, _3p


@Native float Math.sin(float v) {
	return FFI.call(ffi:sin, (int)_1f));
}
@Native Dir opendir(String path, NameSpace _, Dir _)
	return convert(_3p, FFI.call(convertToPath(_2p, )
	using ffi:opendir(path:(1,2)=>Dir:3)


int Test.getNval() {
	int n = this.getRawData(0);
}

int Test.getName() {
	return this.getRawData2(0.25);
}



