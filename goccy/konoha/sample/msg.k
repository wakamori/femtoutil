using konoha.socket.*;

void sendI(Socket s)
{
	OutputStream o = s.getOutputStream();
	int i = 5;
	o.writeObject(i);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendF(Socket s)
{
	OutputStream o = s.getOutputStream();
	float f = 3.14;
	o.writeObject(f);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendB(Socket s)
{
	OutputStream o = s.getOutputStream();
	boolean b = true;
	o.writeObject(b);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendS(Socket s)
{
	OutputStream o = s.getOutputStream();
	String s = "hello world";
	o.writeObject(s);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendIArray(Socket s)
{
	OutputStream o = s.getOutputStream();
	int[] a_i = [2, 4, 5];
	o.writeObject(a_i);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendFArray(Socket s)
{
	OutputStream o = s.getOutputStream();
	float[] a_f = [1.2, 3.4, 5.6];
	o.writeObject(a_f);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendBArray(Socket s)
{
	OutputStream o = s.getOutputStream();
	boolean[] a_b = [true, false, true, false];
	o.writeObject(a_b);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendSArray(Socket s)
{
	OutputStream o = s.getOutputStream();
	String[] a_s = ["h", "e", "l", "l", "o"];
	o.writeObject(a_s);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

class Person {
	String name;
	int age;
	Array<String> habit;
}

void sendPerson(Socket s)
{
	OutputStream o = s.getOutputStream();
	Person p = new Person();
	p.name = "iga";
	p.age = 26;
	p.habit = ["programming", "instrument"];
	o.writeObject(p);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void sendPersonArray(Socket s)
{
	OutputStream o = s.getOutputStream();
	Person p = new Person();
	p.name = "iga";
	p.age = 26;
	p.habit = ["programming", "instrument"];
	a = [];
	a.add(p);
	a.add(p);
	a.add(p);
	o.writeObject(a);
	o.flush();
	OUT <<< "flushed" <<< EOL;
	o.close();
}

void main(String[] args) {
	a = [sendI, sendF, sendB, sendS,
		 sendIArray, sendFArray, sendBArray, sendSArray,
		 sendPerson, sendPersonArray];
	foreach (f in a..) {
		Socket s = new Socket("127.0.0.1", 12345);
		OUT <<< "client is running..." <<< EOL;
		f(s);
		s.close();
	}
}
