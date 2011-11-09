using konoha.proc.*;

void invoke_cpustress() {
	p = new Proc(["/home/tsunade/fault/cpustress-1.0.0/cpustress", "5"]);
	OUT << p.isAlive() << EOL;
}
