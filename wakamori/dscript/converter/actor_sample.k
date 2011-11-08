using konoha.actor.*;

a = actor:localhost:2000;

a.startTask("taskname");
a.eval("""	print "hello";
	print "hi";
""");
a.endTask();

