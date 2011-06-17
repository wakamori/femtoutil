include nativelink;

dynamic InputStream.readObject(Class type, NameSpace ns);
void OutputStream.writeObject(dynamic obj, NameSpace ns);

using konoha.socket.*;

class ConnectionObject {
	//String actor_name; // for "sender" keyword
	String mtd_name;
	dynamic msg;
}

class CActor;
CActor CActor.new(String actor_name);
CActor CActor.setMethodInfo();
void CActor.startDeliver(Func<CActor=>void> f);
void CActor.startScheduler();
void CActor.addMessageToMailBox(String mtd_name, dyn msg);
//void CActor.addMessageToMailBox(String actor_name, String mtd_name, dyn msg);
String CActor.getPathByActorName(String actor_name);
String CActor.getName();
String CActor.getPath();
int CActor.getPort();
String CActor.getOriginalScript(Array<Func<dynamic>> func_list);
String CActor.getScriptPath();
boolean CActor.existsWorkingDir(String dir_name);
boolean CActor.changeToWorkingDir(String dir_name);
void CActor.activateActor(String script_name);

class Message {
	ConnectionObject c;
	String target;
}

Message[] msgs;

class Actor {
	String actor_name;
	CActor a;

	Actor(String actor_name) {
		//_actor_name = actor_name;
		//_a = new CActor(actor_name);
		//_a.setMethodInfo();
	}

	void act(String actor_name) {
		_actor_name = actor_name;
		_a = new CActor(actor_name);
		_a.setMethodInfo();
		_a.startDeliver(delegate(this, deliveryThread));
		_a.startScheduler();
	}

	void deliveryThread(CActor a) {
		//print "start Delivery Thread";
		String name = a.name;
		int port = a.port;
		String path = a.path;
		OUT <<< "======== <<< Actor Information >>> ========" <<< EOL;
		OUT <<< "name : " + name <<< EOL;
		OUT <<< "path : " + path <<< EOL;
		OUT <<< "port : " + port <<< EOL;
		OUT <<< "===========================================" <<< EOL;
		ServerSocket ss = new ServerSocket(port, 3);
		while (true) {
			Socket s = ss.accept();
			ins = s.getInputStream();
			ConnectObject c = ins.readObject(ConnectionObject);
			//print c.msg;
			a.addMessageToMailBox(c.mtd_name, c.msg);
			//a.addMessageToMailBox(c.actor_name, c.mtd_name, c.msg);
			ins.close();
			s.close();
		}
	}
	
	/*
	void send(String target, String mtd_name, dyn msg) {
		ConnectObject c = new ConnectObject();
		c.mtd_name = mtd_name;
		c.msg = msg;
		Message m = new Message();
		m.c = c;
		m.target = target;
		msgs.add(m);
	}

	void delayedSend() {
		for (int i = 0; i < |msgs|; i++) {
			String target = msgs[i].target;
			ConnectObject c = msgs[i].c;
			String actor_path = CActor.getPathByActorName(target);
			String[] param = actor_path.split(":");
			String path =  param[0];
			int port = (int)param[1];
			OUT <<< "SEND : msg >>> [ name : ";
			OUT <<< target + ", path : ";
			OUT <<< path + ", port : " + port + " ]" <<< EOL;
			Socket s = new Socket("127.0.0.1", port);
			OutputStream o = s.getOutputStream();
			o.writeObject(c);
			o.close();
			s.close();
		}
	}
	*/
	
	void sendToActor(String target, String mtd_name, dynamic msg) {
		String actor_path = CActor.getPathByActorName(target);
		String[] param = actor_path.split(":");
		String path =  param[0];
		int port = (int)param[1];
		//print msg.class;
		//print msg;
		OUT <<< "SEND : msg >>> [ name : ";
		OUT <<< target + ", path : ";
		OUT <<< path + ", port : " + port;
		OUT <<< ", mtd(msg): " + mtd_name;
		//OUT <<< "(" + (String)msg + ")";
		OUT <<< " ]" <<< EOL;
		Socket s = new Socket("127.0.0.1", port);
		OutputStream o = s.getOutputStream();
		ConnectionObject c = new ConnectObject();
		//c.actor_name = _actor_name;//for "sender" keyword
		c.mtd_name = mtd_name;
		c.msg = msg;
		o.writeObject(c);
		o.close();
		s.close();
	}
	
	void spawn(String actor_name, Array<Func<dynamic=>void>> func_list) {
		String path = CActor.getScriptPath();
		int idx = path.lastIndexOf(".k");
		String dir = path[0 to idx - 1];
		if (!CActor.existsWorkingDir(dir)) {
			print "create dir";
			System.exec("mkdir " + dir);
		}
		if (!CActor.changeToWorkingDir(dir)) {
			print "ERROR: cannnot change directory";
		}
		//print path;
		print actor_name;
		String script_name = actor_name + ".k";
		print script_name;
		OutputStream writer = new OutputStream(script_name);
		String script = "using konoha.actor.*;\n";
		script += CActor.getOriginalScript(func_list);
		script += "Actor.act(\"" + actor_name + "\");\n";
		//print script;
		writer.print(script);
		writer.close();
		CActor.activateActor(actor_name);
		//CActor.activateActor(script_name);
	}
}
