/*
 * restart_apache.k : Restart apache recovery script
 * @chen_ji <wakamori111@gmail.com>
 */

using konoha.proc.*;

//CONF_FILE="/etc/apache2/apache2.conf";
CONF_FILE="apache2.conf";

class ApacheConf
{
	InputStream ins;
	OutputStream ous;
	String path;

	ApacheConf(String path) {
		_path = path;
	}

	boolean modifyLine(Func<String=>boolean> isMatch, Regex re, String s) {
		boolean replaced = false;
		ins = new InputStream(path, "r");
		String[] buf = [];
		foreach (String line in ins) {
			if (isMatch(line)) {
				line = line.replace(re, s);
				replaced = true;
			}
			buf.add(line);
		}
		ins.close();
		ous = new OutputStream(path, "w");
		foreach (String line in buf) {
			ous << line << EOL;
		}
		ous.flush();
		ous.close();
		return replaced;
	}

	boolean setMaxClients(String module, int max) {
		boolean replaced = false;
		boolean insidetag = false;
		ins = new InputStream(path, "r");
		String[] buf = [];
		foreach (String line in ins) {
			if (line.trim().startsWith("MaxClients") && insidetag && !replaced) {
				line = line.replace($/\d+/, (to String)max);
				replaced = true;
				insidetag = false;
			}
			else if (|line.match(module)| > 0) {
				insidetag = true;
			}
			buf.add(line);
		}
		ins.close();
		ous = new OutputStream(path, "w");
		foreach (String line in buf) {
			ous << line << EOL;
		}
		ous.flush();
		ous.close();
		return replaced;
	}

	boolean setMaxConnection(String from_host, int from_ip, int max) {
		Func<String=>boolean> f = function(String line) {
			return line.trim().startsWith("MaxConnection");
		};
		from_host = (from_host == "") ? "all" : from_host;
		String from_ipstr = (from_ip == 0) ? " " : "/" + (to String)from_ip + " ";
		String s = "MaxConnection " + from_host + from_ipstr + (to String)max;
		return modifyLine(f, $/MaxConnection [^\s]+? \d+?$/, s);
	}
}

boolean restart() {
	Proc p = new Proc(["/usr/sbin/apache2ctl", "restart"]);
	InputStream out = p.getInputStream();
	boolean successed = true;
	foreach (String s in out) {
		if (s != "") {
			successed = false;
		}
		OUT << s << EOL;
	}
	InputStream err = p.getErrorInputStream();
	foreach (String s in err) {
		ERR << s << EOL;
	}
	p.wait();
	return successed;
}

void main(String[] args) {
	ApacheConf aconf = new ApacheConf(CONF_FILE);
	//if (aconf.setMaxConnection("hoge", 0, 20)) {
	//	print "Apache MaxConnection configured successfully";
	//}
	if (aconf.setMaxClients("mpm_worker_module", 12345)) {
		print "Apache MaxClients configured successfully";
	}
	if (restart()) {
		print "Apache restarted successfully";
	}
}
