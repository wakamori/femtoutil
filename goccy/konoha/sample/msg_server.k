using konoha.socket.*;

void receiveI(Socket sock)
{
	i = sock.getInputStream();
	int d = i.readObject(int);
	print d;
}

void receiveF(Socket sock)
{
	i = sock.getInputStream();
	float d = i.readObject(float);
	print d;
}

void receiveB(Socket sock)
{
	i = sock.getInputStream();
	print i.readObject(boolean);
	//boolean b = i.readObject(boolean);
	//print b;//BUG: false
}

void receiveS(Socket sock)
{
	i = sock.getInputStream();
	String s = i.readObject(String);
	print s;
}

void receiveIArray(Socket sock)
{
	i = sock.getInputStream();
	int[] a = i.readObject(Array<int>);
	print a;
}

void receiveFArray(Socket sock)
{
	i = sock.getInputStream();
	float[] a = i.readObject(Array<float>);
	print a;
}

void receiveBArray(Socket sock)
{
	i = sock.getInputStream();
	boolean[] a = i.readObject(Array<boolean>);
	print a;
}

void receiveSArray(Socket sock)
{
	i = sock.getInputStream();
	String[] a = i.readObject(Array<String>);
	print a;
}

class Person {
	int age;
	String name;
	Array<String> habit;
}

void receivePerson(Socket sock)
{
	i = sock.getInputStream();
	Person p = i.readObject(Person);
	print p;
}

void receivePersonArray(Socket sock)
{
	i = sock.getInputStream();
	Person[] a = i.readObject(Array<Person>);
	print a;
}

void main (String[] args) {
	int port = (|args| < 2) ? 12345 : (int) args[1];
	OUT <<< "port=" + port <<< EOL;
	ServerSocket s = new ServerSocket(port, 2);
	a = [receiveI, receiveF, receiveB, receiveS,
		 receiveIArray, receiveFArray, receiveBArray, receiveSArray,
		 receivePerson, receivePersonArray];
	for (int i = 0; i < |a|; i++) {
		Socket csock = s.accept();
		OUT <<< "accepted" <<< EOL;
		a[i](csock);
		csock.close();
		if (i == |a| - 1) i = -1;
		//print i;
	}
	s.close();
}
