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
		if (!rs.next()) return null;
		return rs.getString(0);
	}

	string getUid(string sid) {
		return selectRecord("uid", sid);
	}

	string getScreenName(string sid) {
		return selectRecord("screen_name", sid);
	}

	string getRole(string uid) {
		string query = "select role from " + usr_tbl + " where uid=\"" + uid + "\";";
		ResultSet rs = con.query(query);
		if (!rs.next()) return null;
		return rs.getString(0);
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

	void updateUserRole(string uid, string role) {
		string query = "update " + usr_tbl + " set role=\"" + role + "\" where uid=\"" + uid + "\"";
		con.exec(query);
	}

	string generateSid() {
		return Uuid.getUuid4();
	}

	//Tuple<string, string> getAccessToken(string sid) {
	//	string query = "select access_token, access_token_secret from " + usr_tbl + " where sid=\"" + sid + "\"";
	//	ResultSet rs = con.query(query);
	//	if (!rs.next()) return null;
	//	return (rs.getString(0), rs.getString(1));
	//}

	Tuple<string, string, string>[] getUserList() {
		string query = "select uid, screen_name, role from " + usr_tbl + ";";
		ResultSet rs = con.query(query);
		Tuple<string, string, string>[] ret = [];
		while (rs.next()) {
			ret.add((rs.getString(0), rs.getString(1), rs.getString(2)));
		}
		return ret;
	}

	boolean createUserWithToken(string uid, string screen_name,
			string access_token, string access_token_secret) {
		string query = "insert into " + usr_tbl + " values(\"";
			query += uid + "\",";
			query += "\"\",\""; // sid
			query += screen_name + "\",\"";
			query += access_token + "\",\"";
			query += access_token_secret + "\", \"";
			query += "User" + "\")";
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
