using konoha.actor.*;

void ping(String msg)
{
	OUT <<< msg + " world" <<< EOL;
	Actor.sendToActor("actor1", "recv", "pong");
}

void hoge(int i)
{
	//OUT <<< i + 3 <<< EOL;
}

Actor a = new Actor("actor2");
a.act();
