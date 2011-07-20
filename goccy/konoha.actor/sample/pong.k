using konoha.actor.*;

//void ping(String msg)
void ping(RingMessage msg)
{
	//msg += " world";
	//print msg;
	Actor.send("actor1", "pong", "hello world");
	//Actor.send("actor1", "pong", msg);
}


void hoge(int i)
{
	//OUT <<< i + 3 <<< EOL;
}

Actor.act("actor2");
