using konoha.actor.*;

int main(String[] args)
{
	a1 = actor:192.168.0.203:50010;
	ins = new InputStream("proc.k");
	buf = "";
	foreach (String line in ins) {
		if (line == "") continue;
		buf = buf + line + EOL;
	}
	a1.eval(buf);

	ins = new InputStream("apache_search.k");
	buf = "";
	foreach (String line in ins) {
		if (line == "") continue;
		buf = buf + line + EOL;
	}
	a1.eval(buf);
	a1.eval("apache_search();");
	a2 = actor:192.168.0.203:50011;
	ins = new InputStream("memstress.k");
	buf = "";
	foreach (String line in ins) {
		if (line == "") continue;
		buf = buf + line + EOL;
	}
	a2.eval(buf);
	OUT << buf << EOL;
	a2.eval("invoke_memstress();");
	return 0;
}
