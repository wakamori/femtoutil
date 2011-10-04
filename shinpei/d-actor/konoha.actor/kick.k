using konoha.actor.*;

void process_done() {
	print "done";
}

int main (String[] args) {
	a = actor:localhost:2000;
	print a;
	a.eval("""
void f() {
    OUT <<< "hello world" <<< EOL;
}""");
	a.eval("f()");
	return 0;
/*

	a1 = actor:192.168.0.5:2000;
	a2 = actor:192.168.0.6:2000;

	a1.eval("""
using konoha.actor.*;
using konoha.posix.*;

a2 = actor:192.168.0.6:2000;

void process_a (int i) {
  for(int i = 0; i < 5; i++) {
    OUT << ". " << EOL;
    sleep(1);
  }
  a2.eval("process_b()");
}
""");

	a2.eval("""
using konoha.actor.*;
using konoha.posix.*;

a0 = actor:192.168.0.2:2000;

void process_b (int i) {
  for(int i = 0; i < 5; i++) {
    OUT << ". " << EOL;
    sleep(1);
  }
  a0.eval("process_done();");
}
""");

	a1.eval("process_a()");
	Actor.startActorMyself(2000);
  	return 0;
*/
}
