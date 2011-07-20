include "lib:gluelink";

class Actor;
class ConnectionObject {
	String mtd_name;
	dynamic msg;
};

void Actor.act(String actor_name);
void Actor.send(String target_name, String mtd_name, dynamic msg);
void Actor.spawn(String actor_name, Array<Func<dynamic>> func_list);

class Person {
	String name;
	int age;
	Person(String name, int age) {
		_name = name;
		_age = age;
	}
}

class RingMessage {
	String name;
	int actor_num;
	int actor_count;
	int loop_count;
	RingMessage(String name, int num1, int num2, int num3) {
		_name = name;
		_actor_num = num1;
		_actor_count = num2;
		_loop_count = num3;
	}
}
