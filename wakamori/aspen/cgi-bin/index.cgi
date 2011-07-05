#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgitb; cgitb.enable()
import login
import os
import ConfigParser
import Cookie

html_login = '''<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>Aspen - An online KonohaScript editor</title>
		<link rel="stylesheet" href="%(rootpath)s/css/aspen.css" />
		<script type="text/javascript" src="%(rootpath)s/js/jquery-1.6.1.min.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/jquery.lightbox_me.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/login.js"></script>
	</head>
	<body>
		<form id="loginform" name="loginform" action="%(cgipath)s/aspen.cgi?method=login" method="post">
			<div class="info">Aspenを使用するには、Twitterアカウントでの認証が必要です。</div>
			<input type="image" src="%(rootpath)s/img/sign-in-with-twitter-d.png" alt="Twitterで認証する" align="right">
		</form>
	</body>
</html>'''

html_main = '''<!DOCTYPE html>
<html>
	<head>
		<title>Aspen - An online KonohaScript editor</title>
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
		<script type="text/javascript" src="%(rootpath)s/js/codemirror.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/autocompletion.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/mootools-core-1.3.2-full-compat.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/mootools-more-1.3.2.1.js"></script>
		<script type="text/javascript" src="%(rootpath)s/js/aspen.js"></script>
		<script type="text/javascript" src="%(rootpath)s/mode/konoha/konoha.js"></script>
		<link rel="stylesheet" href="%(rootpath)s/css/aspen.css" />
		<link rel="stylesheet" href="%(rootpath)s/css/codemirror.css" />
		<link rel="stylesheet" href="%(rootpath)s/mode/konoha/konoha.css" />
	</head>
	<body>
		<table role="presentation">
			<tr>
				<td class="logo">
					<a href="%(indexpath)s"><img class="logo" src="%(rootpath)s/img/konoha_logo.png" alt="Konoha"/></a>
				</td>
				<td>
					<div class="info">
						<span class="user">Hello, %(name)s.</span>
					</div>
					<form id="logoutform" class="inlineform" name="logoutform" action="%(cgipath)s/aspen.cgi" method="get">
						<input type="hidden" name="method" value="logout" />
						<input class="normalbutton" type="submit" id="logout" value="Log out" />
					</form>
					<!--
					<form id="newform" class="inlineform" name="newform" action="%(cgipath)s/aspen.cgi" method="get">
						<input type="hidden" name="method" value="new" />
						<input class="normalbutton" type="submit" id="new" value="New" />
					</form>
					-->
					<input type="button" class="fixedbutton" id="new" name="new" value="New" />
					<input type="button" class="fixedbutton" id="eval" name="eval" value="Run" />
<!--				<input type="button" class="fixedbutton" id="save" alt="save the current buffer at remote server" value="Save" />
					<input type="button" class="fixedbutton" id="load" alt="load a script from your server" value="Load" />
					<input type="button" class="fixedbutton" id="save" alt="open your script" value="Open" onclick='javascript:{ if ($("#file").css("display") == "none") { $("#file").css("display", "inline");} else { $("#file").css("display", "none"); }};' />
					<input style="display:none" type="file" id="file" name="file" />-->
				</td>
			</tr>
		</table>

		<!-- tabs -->
		<!--
		<div id="doclist">
			<h2>Documents</h2>
			<ul id="documents">
				<li><a href="#" rel="Document1" title="This is the content of Document1">Document1</a></li>
				<li><a href="#" rel="Document2" title="This is the content of Document2">Document2</a></li>
				<li><a href="#" rel="Document3" title="This is the content of Document3">Document3</a></li>
				<li><a href="#" rel="Document4" title="This is the content of Document4">Document4</a></li>
				<li><a href="#" rel="Document5" title="This is the content of Document5">Document5</a></li>
			</ul>
		</div>
		<div id="wrapper">
			<ul id="tabs">
			</ul>
			<div id="content">
			</div>
		</div>
		-->
		<div id="result">
			<span class="message">Result will be displayed here.</span>
		</div>
<div>
						<input class="headbutton" type="submit" id="rewind" value=" < " />
						<input class="headbutton" type="submit" id="forward" value=" > " />
</div>
		<textarea id="code" name="code" rows="30" cols="80"></textarea>
		<div id="status">
		</div>
		<div class="info">
			このエディタに関するご質問、ご意見は<img src="%(rootpath)s/img/gmail.png" id="mail" alt="wakamori111 at gmail.com"/>までお願いします。
		</div>
	</body>
</html>'''

def main():
	conf = ConfigParser.SafeConfigParser()
	conf.read('settings.ini')
	rootpath = conf.get('general', 'rootpath')
	cgipath = conf.get('general', 'cgipath')
	indexpath = conf.get('general', 'indexpath')
	print 'Content-Type: text/html;charset=UTF-8\n'
	cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
	if cookie.has_key('access_token') and cookie.has_key('access_token_secret'):
		lm = login.LoginManager()
		try:
			name = lm.getAccountInfo(cookie['access_token'].value,
				cookie['access_token_secret'].value)['name'].encode('utf-8')
			print html_main % locals()
		except Exception:
			# not authorized
			print html_login % locals()
	else:
		print html_login % locals()

if __name__ == '__main__':
	main()
