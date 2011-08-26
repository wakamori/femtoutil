#!/usr/local/bin/konoha

/*
 * aspen.k : small script for invoking konoha
 * (c) shinpei_NKT, utrhira, chen_ji
 */

include "db.k";
include "login.k";
//include "bugreporter.k"
//import signal
using konoha.json.*;
using konoha.cookie.*;

KONOHA_PATH = "/Users/waka/Developments/konoha";
GIT_PATH    = "/Users/waka/Developments/femtoutil/.git";
BASE_PATH   = "data";
SCRIPT_DIR  = "scripts";
BUG_DIR     = "bugs";
TMP_DIR     = "tmp";

class Aspen
{
	Storage storage;
	string uid;
	string base;
	string bug_dir;
	string scr_dir;
	string tmp_dir;

	Aspen(string base_path, string bug_dir, string script_dir string tmp_dir) {
		_storage = new Storage();
		_uid = "";
		_base = base_path;
		_bug_dir = bug_dir;
		_scr_dir = script_dir;
		_tmp_dir = tmp_dir;
	}

	void printText(string text) {
		OUT << "Content-Type: text/plain\n" << EOL;
		OUT << text << EOL;
	}

	void deleteCookie(string key) {
		string past = "Thu, 01 Jan 1970 00:00:00 GMT";
		Cookie cookie = new Cookie();
		cookie[key] = "";
		cookie[key]["path"] = "/";
		cookie[key]["expires"] = past;
		OUT << cookie.dump();
	}

	void saveCookie(string key, string val) {
		Cookie cookie = new Cookie();
		cookie[key] = val;
		cookie[key]["path"] = "/";
		OUT << cookie.dump();
	}

	void auth(string sid) {
		uid = storage.getUid(sid);
		if (uid == "") {
			throw new Script!!("Authentication failed.");
		}
	}

	void evalScript(string scr_name, string scr_body) {
	}

	void signoutOauth() {
		deleteCookie("SID");
		OUT << EOL;
	}

	void printScript(string scr_name) {
		OUT << "Content-Type: text/plain\n" << EOL;
		string scr_path = base + "/" + scr_dir + "/" + uid + "/" + scr_name;
		InputStream is = new InputStream(scr_path, "r");
		if (is != null) {
			foreach (string s in is) {
				OUT << s << EOL;
			}
		}
	}

	void printFileList() {
		string usr_dir = base + "/" + scr_dir + "/" + uid;
		Json j = new Json();
		String[] files = exec("ls " + usr_dir).split("\n");
		files.pop();
		j["item"] = [["/", [], files]];

		OUT << "Content-Type: application/json;charset=UTF-8\n" << EOL;
		OUT << j.stringify() << EOL;
		//OUT << "Content-Type: text/html\n" << EOL;
		//OUT << "files=" + files << EOL;
		//OUT << "usr_dir=" + usr_dir << EOL;
		//OUT << "pwd=" + exec("pwd") << EOL;
	}

	void storeScript(string scr_name, string scr_body) {
		string usr_dir = base + "/" + scr_dir + "/" + uid;
		if (new InputStream(usr_dir, "r") == null) {
			exec("mkdir " + usr_dir);
		}
		string scr_path = usr_dir + "/" + scr_name;
		OutputStream script = new OutputStream(scr_path, "w");
		script << scr_body;
		script.close();
		printText("Script \"" + scr_name + "\" was saved successfully.");
	}
}

void main(string[] args)
{
	//OUT << a.cookie << EOL;
	//OUT << $env.QUERY_STRING << EOL;
	////string method = $env."REQUEST_METHOD";
	////OUT << method << EOL;
	//OUT << "Content-Type: text/html\n" << EOL;
	Aspen a = new Aspen(BASE_PATH, BUG_DIR, SCRIPT_DIR, TMP_DIR);
	Cookie cookie = new Cookie($env.HTTP_COOKIE);
	Map<string,string> field;
	string method = $env.REQUEST_METHOD;
	if (method == "POST") {
		Bytes b = new byte[(int)$env.CONTENT_LENGTH];
		IN.read(b, 0, b.getSize());
		field = parseQuery(b.decode());
		switch (field["type"]) {
		case "eval":
			a.auth(cookie["SID"]["val"]);
			a.evalScript(field["name"], field["kscript"]);
			break;
		case "save":
			a.auth(cookie["SID"]["val"]);
			a.storeScript(field["name"], field["kscript"]);
			break;
		default:
			throw new Script!!("POST does not have type " + field["type"] + ".");
			break;
		}
	} else if (method == "GET") {
		field = parseQuery($env.QUERY_STRING);
		switch (field["type"]) {
		case "signout":
			a.signoutOauth();
			break;
		case "load":
			a.auth(cookie["SID"]["val"]);
			a.printScript(field["file"]);
			break;
		case "open":
			a.auth(cookie["SID"]["val"]);
			a.printFileList();
			break;
		default:
			throw new Script!!("GET does not have type " + field["type"] + ".");
			break;
		}
	} else {
		throw new Script!!("No such method.");
	}
}
