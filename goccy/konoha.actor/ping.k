using konoha.actor.*;

void recv(String msg)
{
	OUT <<< msg <<< EOL;
}

Actor a = new Actor("actor1");
a.sendToActor("actor2", "ping", "hello");
a.sendToActor("actor2", "hoge", 4);
a.act();
