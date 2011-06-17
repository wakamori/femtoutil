using konoha.actor.*;

void ping(String msg)
{
	msg += " world";
	//print msg;
	Actor.sendToActor("actor1", "pong", "hello world");
	//Actor.sendToActor("actor1", "pong", msg);
}


void hoge(int i)
{
	//OUT <<< i + 3 <<< EOL;
}

Actor.act("actor2");
