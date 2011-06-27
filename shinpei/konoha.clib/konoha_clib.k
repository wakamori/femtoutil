// konoha microbench by kimio kuramitsu

using konoha.clib.*;
Clib math = new Clib("/usr/lib/libm.dylib");
Glue g = math.genGlue();
Func<Float=>Float> fabs = g.getFunc("fabs");

int testMathFabs() {
	int i;
	for(i = 0; i< 100_000_000; i++) {
		fabs(-1.0);
	}
	return i;
}

float mops(int score) {
	if(score < 1) return 0;
	return (10000000 / score) / 100.0;
}


void run() {
	s = System.getTime();
	t = System.getTime() - s;
	score = t;
	delta = t;

	s = System.getTime();
	testMathFabs();
	t = System.getTime() - s;
	score = t - delta;
	print "NativeFunction[mops,ms]:", mops(score), t;
	score = t;
	print "fibonacci(36)[mops,ms]:", mops(score), t;
}

run();



