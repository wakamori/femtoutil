using konoha.actor.*;

void start()
{
	//p = new Person("yoan", 23);
	r = new RingMessage("name", 1, 2, 3);
	Actor.send("actor2", "ping", r);
	//Actor.sendToActor("actor2", "hoge", 4);
}

void pong(String msg)
{
	OUT <<< msg <<< EOL;
}

Actor.act("actor1");
