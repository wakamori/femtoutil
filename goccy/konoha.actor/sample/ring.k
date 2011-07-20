using konoha.actor.*;
using konoha.posix.*;

int i;

void endAll(String msg) {
	//print "endall";
	print System.getTime() - i;
	//CActor.exit();
}

void exitRing(RingMessage msg)
{
	if (msg.actor_num != msg.actor_count) {
		msg.actor_num += 1;
		Actor.sendToActor((string)(msg.name + msg.actor_num), "exitRing", msg);
	} else {
		Actor.sendToActor("kick", "endAll", "");
		//print "endAll";
	}
	CActor.exit();
}

void receive(RingMessage msg)
{
	//print "receive!!";
	//print msg;
	if (msg.actor_num == msg.actor_count) {
		msg.loop_count -= 1;
		msg.actor_num = 1;
		if (msg.loop_count == 0) {
			Actor.sendToActor("actor1", "exitRing", msg);
		}
	} else {
		msg.actor_num += 1;
	}
	String actor_name = msg.name + msg.actor_num;
	//print actor_name;
	//print msg;
	Actor.sendToActor(actor_name, "receive", msg);
}

void main(String[] args)
{
	int actor_count = 3;
	int loop_count = 10;
	if (|args| == 2) {
		actor_count = (int)args[0];
		loop_count = (int)args[1];
	}
	Array<Func<dynamic=>void>> func_list = [];
	func_list.add(receive);
	func_list.add(exitRing);
	for (int i = 1; i <= actor_count; i++) {
		String name = "actor" + i;
		Actor.spawn(name, func_list);//generate script
		sleep(1);
	}
	i = System.getTime();   
	Actor.sendToActor("actor1", "receive", new RingMessage("actor", 1, actor_count, loop_count));
	Actor.act("kick");
}
