#!/usr/local/bin/konoha

/*
 * storage.k : for storing user data
 * (c) shinpei_NKT, chen_ji
 */

INCLUDE_STORAGE = true;

using konoha.uuid.*;

DB_NAME         = "";
USER_TABLE_NAME = "";

class Storage
{
	string usr_tbl;
	Connection con;

	Storage() {
		_usr_tbl = USER_TABLE_NAME;
		_con = new Connection("konoha.sqlite3:" + DB_NAME);
	}

	boolean hasUser(string uid) {
		string query = "select * from " + usr_tbl + " where uid=\"" + uid + "\"";
		ResultSet rs = con.query(query);
		return rs.next();
	}

	string selectRecord(string record, string sid) {
		string query = "select " + record + " from " + usr_tbl + " where sid=\"" + sid + "\"";
		ResultSet rs = con.query(query);
		if (!rs.next()) return "";
		return rs[0];
	}

	string getUid(string sid) {
		return selectRecord("uid", sid);
	}

	string getScreenName(string sid) {
		return selectRecord("screen_name", sid);
	}

	string renewSession(string uid) {
		string sid = generateSid();
		string query = "update " + usr_tbl + " set sid=\"" + sid + "\" where uid=\"" + uid + "\"";
		//print query;
		//r = con.query("select sid from users where uid=" + uid);
		//r.next();
		//print r[0];
		con.exec(query);
		//r = con.query("select sid from users where uid=" + uid);
		//r.next();
		//print r[0];
		return sid;
	}

	string generateSid() {
		return Uuid.getUuid4();
	}

	Tuple<String, String> getAccessToken(string sid) {
		return (selectRecord("access_token", sid),
				selectRecord("access_token_secret", sid));
	}

	boolean createUserWithToken(string uid, string screen_name,
			string access_token, string access_token_secret) {
		string query = "insert into " + usr_tbl + " values(\"";
			query += uid + "\",";
			query += "\"\",\""; // sid
			query += screen_name + "\",\"";
			query += access_token + "\",\"";
			query += access_token_secret + "\")";
		try {
			con.exec(query);
		} catch (Exception!! e) {
			ERR << "ERROR: " + e << EOL;
			ERR << uid + "is probably already exists in database." << EOL;
			return false;
		}
		return true;
	}
}

void main(string[] args)
{
	Storage s = new Storage();
	print s.hasUser("123"); // false
	print s.hasUser("66415991"); // true
}
