using konoha.actor.*;

a = actor:localhost:2000;
a.startTask("foo");
a.eval("print \"hi\"");
a.endTask();
