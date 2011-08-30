#!/usr/local/bin/konoha

/*
 * login.k : OAuth login manager
 * (c) chen_ji
 */

using konoha.liboauth.*;
using konoha.cookie.*;
using konoha.cgi.*;

if (not defined INCLUDE_STORAGE) {
	include "storage.k";
}

REQUEST_TOKEN_URL = "";
ACCESS_TOKEN_URL  = "";
AUTHENTICATE_URL  = "";
CONSUMER_KEY      = "";
CONSUMER_SECRET   = "";
BASE_URL          = "";
DOC_URL           = "";

void redirectToIndex() {
	OUT << "Location: " + BASE_URL + "\n" << EOL;
}

void redirectToDoc() {
	OUT << "Location: " + DOC_URL + "\n" << EOL;
}

class LoginManager
{
	string request_token_url;
	string access_token_url;
	string authenticate_url;
	Consumer consumer;

	LoginManager() {
		_request_token_url = REQUEST_TOKEN_URL;
		_access_token_url = ACCESS_TOKEN_URL;
		_authenticate_url = AUTHENTICATE_URL;
		_consumer = new Consumer(CONSUMER_KEY, CONSUMER_SECRET);
	}

	Map<string,string> getRequestToken() {
		Client client = new Client(consumer);
		Map<string,string> ret = client.request(request_token_url, "GET");
		if (ret.getSize() == 0) {
			throw new Script!!("request failed.");
		}
		return ret;
	}

	void redirectToProvider(string oauth_token) {
		OUT << "Location: " + authenticate_url + "?oauth_token=" + oauth_token + "\n" << EOL;
	}

	void saveRequestCookie(Map<string,string> rtoken) {
		Cookie cookie = new Cookie();
		cookie["request_token"] = rtoken["oauth_token"];
		cookie["request_token"]["path"] = "/";
		cookie["request_token_secret"] = rtoken["oauth_token_secret"];
		cookie["request_token_secret"]["path"] = "/";
		OUT << cookie.dump();
	}

	Map<string,string> getAccessToken(string rtoken, string rtoken_secret) {
		Map<string,string> field = Cgi.parseQuery($env.QUERY_STRING);
		if (field["denied"] != null) {
			redirectToIndex();
		}
		RequestToken token = new RequestToken(rtoken, rtoken_secret);
		token.setVerifier(field["oauth_verifier"]);
		Client client = new Client(consumer, token);
		Map<string,string> ret = client.request(access_token_url, "GET");
		if (ret.getSize() == 0) {
			throw new Script!!("request failed.");
		}
		return ret;
	}
}

void main()
{
	//OUT << "Content-Type: text/html\n" << EOL;
	//		print "ERROR!!";
	Cookie cookie = new Cookie($env.HTTP_COOKIE);
	LoginManager manager = new LoginManager();
	Map<string,string> m = manager.getAccessToken(cookie["request_token"]["val"],
			cookie["request_token_secret"]["val"]);

	string uid = m["user_id"];
	string screen_name = m["screen_name"];
	if (uid == null) {
		// oauth failed
		OUT << "Content-Type: text/plain\n" << EOL;
		throw new Script!!("OAuth failed.");
	}
	Storage storage = new Storage();

	if (not storage.hasUser(uid)) {
		storage.createUserWithToken(uid, screen_name, m["oauth_token"], m["oauth_token_secret"]);
	}

	string sid = storage.renewSession(uid);

	cookie = new Cookie();
	cookie["SID"] = sid;
	cookie["SID"]["path"] = "/";

	string past = "Thu, 01 Jan 1970 00:00:00 GMT";

	cookie["request_token"] = "dummy";
	cookie["request_token"]["path"] = "/";
	cookie["request_token"]["expires"] = past;
	cookie["request_token_secret"] = "dummy";
	cookie["request_token_secret"]["path"] = "/";
	cookie["request_token_secret"]["expires"] = past;

	OUT << cookie.dump();
	redirectToDoc();
}
