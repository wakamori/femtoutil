
include lib:gluelink;

dynamic InputStream.readObject(Class type, NameSpace ns);
void OutputStream.writeObject(dynamic obj, NameSpace ns);

using konoha.socket.*;

class ConnectionObject {
	//String actor_name; // for "sender" keyword
	String mtd_name;
	dynamic msg;
}

class Person {
	String name;
	int age;
	Person(String name, int age) {
		_name = name;
		_age = age;
	}
}

class RingMessage
{
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

class CActor;
CActor CActor.new(String actor_name);
CActor CActor.setMethodInfo();
void CActor.startDeliver(Func<CActor=>void> f);
void CActor.startScheduler();
void CActor.addMessageToMailBox(String mtd_name, dynamic msg);
//void CActor.addMessageToMailBox(String actor_name, String mtd_name, dyn msg);
String CActor.getPathByActorName(String actor_name);
String CActor.getName();
String CActor.getPath();
int CActor.getPort();
String CActor.getOriginalScript(Array<Func<dynamic>> func_list, String actor_name);
String CActor.getScriptPath();
boolean CActor.existsWorkingDir(String dir_name);
boolean CActor.changeToWorkingDir(String dir_name);
void CActor.activateActor(String dir, String script_name);
void CActor.sendToScalaActor(String actor_name, String host, int port, int msg);
void CActor.readMessage(ConnectionObject c);
int CActor.fork();
void CActor.exit();

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
		//_a.startDeliver(delegate(this, deliveryThread));
		//_a.startScheduler();
		deliveryThread(_a);
	}

	void deliveryThread(CActor a) {
		//print "start Delivery Thread";
		//String name = a.name;
		int port = a.port;
		//String path = a.path;
		//OUT <<< "======== <<< Actor Information >>> ========" <<< EOL;
		//OUT <<< "name : " + name <<< EOL;
		//OUT <<< "path : " + path <<< EOL;
		//OUT <<< "port : " + port <<< EOL;
		//OUT <<< "===========================================" <<< EOL;
		ServerSocket ss = new ServerSocket(port, 3);
		while (true) {
			Socket s = ss.accept();
			//print "accept";
			//print s;
			int pid = CActor.fork();
			//print pid;
			if (pid == 0) {
				//child
				//print "child";
				InputStream ins = s.getInputStream();
				cls = ConnectionObject;
				ConnectionObject c = ins.readObject(cls);
				//print c;
				//print c.mtd_name;
				//print c.msg;
				//print a.name;
				a.readMessage(c);
				ins.close();
				a.exit();
			} else {
			}
			s.close();
		}
	}
	
	void invokeMessage(Socket s) {
		InputStream ins = s.getInputStream();
		ConnectionObject c = ins.readObject(ConnectionObject);
		print c.msg;
		//a.addMessageToMailBox(c.mtd_name, c.msg);
		//a.addMessageToMailBox(c.actor_name, c.mtd_name, c.msg);
		ins.close();
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
		//print msg;
		String actor_path = CActor.getPathByActorName(target);
		String[] param = actor_path.split(":");
		//String path =  param[0];
		int port = (int)param[1];
		//print msg.class;
		//print msg;
		// OUT <<< "SEND : msg >>> [ name : ";
		// OUT <<< target + ", path : ";
		// OUT <<< path + ", port : " + port;
		// OUT <<< ", mtd(msg): " + mtd_name;
		// OUT <<< " ]" <<< EOL;
		Socket s = new Socket("127.0.0.1", port);
		OutputStream o = s.getOutputStream();
		ConnectionObject c = new ConnectionObject();
		//c.actor_name = _actor_name;//for "sender" keyword
		c.mtd_name = mtd_name;
		c.msg = msg;
		//print msg;
		//print c;
		//print c.msg;
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
		//print actor_name;
		String script_name = actor_name + ".k";
		//print script_name;
		OutputStream writer = new OutputStream(script_name);
		//String script = "using konoha.actor.*;\n";
		String script = CActor.getOriginalScript(func_list, actor_name);
		//script += "Actor.act(\"" + actor_name + "\");\n";
		//print script;
		writer.print(script);
		writer.close();
		//CActor.activateActor(actor_name);
		CActor.activateActor(dir, script_name);
	}
	
	void request() {
		
	}

	void sendToScalaActor(String actor_name, String host, int port, int msg) {
		CActor.sendToScalaActor(actor_name, host, port, msg);
	}
}
