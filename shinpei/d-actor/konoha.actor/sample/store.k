/*
 * Start a memcached daemon before executing this script.
 * e.g.)
 * $ memcached -m 64m -d
 */

using konoha.actor.*;

sa = new StoreActor("hoge", "localhost", 11211);
sa["int"] = 123;
int i = sa["int"];
print i; // 123

sa["string"] = "abc";
String s = sa["string"];
print s; // "abc"

// TODO
//function f(int n) {
//	print n;
//}
//sa["func"] = f;
//sa["func"](3);
