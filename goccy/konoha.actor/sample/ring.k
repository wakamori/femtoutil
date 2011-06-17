using konoha.actor.*;

void receive(Tuple<String,int,int> msg)
{
	print "receive!!!";
	String name = msg[0];
	int actor_num = msg[1];
	int loop_count = msg[2];
	String actor_name = "";
	if (actor_num == 10) {
		loop_count--;
		actor_num = 1;
	} else {
		actor_num++;
	}
	actor_name = name + actor_num;
	Actor.sendToActor(actor_name, "receive", (name, actor_num, loop_count));
}

void main(String[] args)
{
	Array<Func<dynamic=>void>> func_list = [];
	func_list.add(receive);
	for (int i = 1; i <= 2; i++) {
		String name = "actor" + i;
		Actor.spawn(name, func_list);//generate script
	}
	Actor.sendToActor("actor1", "receive", ("actor", 1, 3));
}
