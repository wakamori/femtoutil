using konoha.proc.*;

void invoke_memstress() {
	//p = new Proc(["/home/tsunade/fault/memstress-1.0.0/memstress", "102400", "170000", "5000", "&"]);
	exec("/home/tsunade/fault/memstress-1.0.0/memstress 102400 17000 50 &");
	//print "hoge";
}
