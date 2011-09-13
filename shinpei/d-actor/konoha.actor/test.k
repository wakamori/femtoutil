using konoha.actor.*;

a = actor:localhost:2000

a.eval("""
using konoha.math.*;

print Math.PI;

void f (int i) {
  print "this is f!!";
}
""");

a.eval("f(3)");
