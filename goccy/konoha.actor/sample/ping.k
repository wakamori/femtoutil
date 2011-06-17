using konoha.actor.*;

void start()
{
	Actor.sendToActor("actor2", "ping", "hello");
	Actor.sendToActor("actor2", "hoge", 4);
}

void pong(String msg)
{
	OUT <<< msg <<< EOL;
}

Actor.act("actor1");
