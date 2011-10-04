/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c)  2010-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

// **************************************************************************
// LIST OF CONTRIBUTERS
//  kimio - Kimio Kuramitsu, Yokohama National University, Japan
//  shinpei_NKT - Shinpei Nakata, Yokohama National University, Japan
//  goccy - Masaaki Goshima, Yokohama National University, Japan
//  chen_ji - Takuma Wakamori, Yokohama National University, Japan
// **************************************************************************

//using konoha.memcached.*;
using konoha.io.*;
using konoha.socket.*;
using konoha.proc.*;

include "lib:gluelink";

@Native class Actor;

/*class StoreActor {
	Map m;
	String name;

	StoreActor(String name, String host, int port) {
		m = Map.open("memcached:" + host + ":" + (to String)port);
		if (m == null) throw new CannotCreateException!!();
		_name = name;
	}

	dynamic get(String key) {
		if (m[key] == null) return null;
		Bytes b;
		if (m[key] <: String)
			b = (to Bytes)(to String)m[key];
		else
			b = m[key];
		return b.readMsgPack(0, |b|);
	}

	void set(String key, Object data) {
		Bytes b = new Bytes();
		b.writeMsgPack(data);
		m[key] = b;
	}
	}*/

@Native @Hidden Actor Actor.opLINK(String path, NameSpace _);
//@Native boolean Actor.setStore(StoreActor store);
//@Native boolean Actor.sendMethod(Func<dynamic=>dynamic> method);

@Native String Actor.getName();
@Native String Actor.getHost();
@Native int Actor.getPort();

//@Native StoreActor StoreActor.new(String name, String host, int port);

//class ConnectionObject {
//	String mtd_name;
//	dynamic msg;
//};

//void Actor.act(String actor_name);
//void Actor.send(String target_name, String mtd_name, dynamic msg);
//void Actor.spawn(String actor_name, Array<Func<dynamic>> func_list);



boolean Actor.eval(String script) {
	Socket s;
	try {
		s = new Socket(this.host, this.port);
		OutputStream ous = s.getOutputStream();
		//TODO: we need \n as a termination term
		ous.writeMsgPack(script + "\n");
		ous.flush();
		ous.close();
		s.close();
	} catch (Exception!! e) {
		print e;
	}
	return true;
}

void Actor.startActorMyself (int port) {
	p = new Proc(["/usr/local/bin/konoha", "mailbox.k", (String)port]);
	print p;
	InputStream ins = p.getInputStream();
	while (p.isAlive()) {
		String msg = ins.readLine();
		System.eval(msg);
	}
/*	int ch;
	string line = '';
	OUT <<< "starting actor mode @port: " + port <<< EOL;
	ServerSocket ss = new ServerSocekt(port, 3);

	while(true) {
		Socket conn = ss.accept();
		OUT << "accepted" << EOL;
		ins = conn.getInputStream();
		String scr = ins.readMsgPack();
		OUT << "received script: '" << scr << "'"<< EOL;
		System.eval(scr);
	}
	OUT << "connection has closed" << EOL;
*/
}



//class Person {
//	String name;
//	int age;
//	Person(String name, int age) {
//		_name = name;
//		_age = age;
//	}
//}

//class RingMessage {
//	String name;
//	int actor_num;
//	int actor_count;
//	int loop_count;
//	RingMessage(String name, int num1, int num2, int num3) {
//		_name = name;
//		_actor_num = num1;
//		_actor_count = num2;
//		_loop_count = num3;
//	}
//}
