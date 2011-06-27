using konoha.actor.*;

class Person {
	String name;
	int age;
	Person(String name, int age) {
		_name = name;
		_age = age;
	}
}

void start()
{
	p = new Person("yoan", 23);
	Actor.sendToActor("actor2", "ping", p);
	//Actor.sendToActor("actor2", "hoge", 4);
}

void pong(String msg)
{
	OUT <<< msg <<< EOL;
}

Actor.act("actor1");
