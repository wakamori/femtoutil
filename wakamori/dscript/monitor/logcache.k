using konoha.memcached.*;
using konoha.json.*;

class Log {
	String time;
	String host;
	int pid;
	String trace;
	String name;
	Map val;

	Log(String time, String host, int pid, String trace, String name, Map val) {
		_time = time;
		_host = host;
		_pid = pid;
		_trace = trace;
		_name = name;
		_val = val;
	}
}

class Logcache {
	Memcache mcd;
	String trace;
	//String log_name;
	int ctxid;
	Map<int> seq;
}

Logcache Logcache.new(String host, int port, String task_name) {
	_mcd = new Memcache(host, port);
	_trace = mcd[task_name];
	if (trace == null) throw new Script!!("cannot trace " + task_name);
	_task_name = task_name;
	//_log_name = log_name;
	_ctxid = 0;
	_seq = {};
}

Log Logcache.readLog() {
	String key;
	String line;
	int i = seq[trace];
	key = trace + "+" + i;
	//print key;
	//print log_name;
	//line = mcd.getByKey(log_name, key);
	line = mcd[key];
	if (line == null) return null;

	//print line;
	String[] e = line.split(" ");
	String logname = e[6];
	//print logname;
	String json = line.substring(line.indexOf(logname) + |logname| + 1);
	//print json;

	//print line;
	seq[trace] += 1;
	//print e[4];
	int port = (to int)e[4].replace($/konoha\[(\d+)\]:/, "$1");
	Map m = ((to Bytes)json).readJson(0, |json|);
	Log l = new Log(line.substring(0, line.indexof(e[3]) - 1), e[3], port, e[5], e[6], m);
	return l;
}
