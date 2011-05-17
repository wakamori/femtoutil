class Person {
	String name;
	int age;
	float height;
	boolean isChild;
	Array<String> habit;
}

class Hoge {
	int id;
	Person p;
}

void main(String[] args)
{
	OutputStream o = new OutputStream("hi");
	int i = 2;
	float f = 3.14;
	boolean b = true;
	String s = "hello world";
	o.writeObject(i);
	o.writeObject(f);
	o.writeObject(b);
	o.writeObject(s);
	str_array = ["h", "e", "l", "l", "o"];
	int_array = [1, 2, 3, 4, 5];
	float_array = [1.2, 3.4, 5.6];
	o.writeObject(str_array);
	o.writeObject(int_array);
	o.writeObject(float_array);
	Person p = new Person();
	p.name = "konoha";
	p.age = 24;
	p.height = 180.5;
	p.habit = ["programming", "instrument"];
	o.writeObject(p);
	Hoge h = new Hoge();
	h.id = 54;
	h.p = p;
	o.writeObject(h);
}
