#!/usr/bin/python
# -*- coding: utf-8 -*-

import Cookie
import os
import cgitb; cgitb.enable()

html_login = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja" lang="ja">
	<head>
		<title>アルゴリズムとデータ構造  - try with konoha </title>
		<link rel="stylesheet" href="../aspen/css/aspen.css" />
		<script type="text/javascript" src="../aspen/js/jquery-1.6.1.min.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.lightbox_me.js"></script>
		<script type="text/javascript" src="../aspen/js/login.js"></script>
	</head>
	<body>
		<form id="loginform" name="loginform" action="./aspen.cgi?method=login" method="post">
			<table summary="Login dialog.">
				<tr><th colspan="2">アルゴリズムとデータ構造</th></tr>
				<tr><td colspan="2">学籍番号とパスワードでログインしてください</td></tr>
				<tr><th>学籍番号</th><td><input type="text" class="sprited" id="username" name="username" value="your name" /></td></tr>
				<tr><th>パスワード</th><td><input type="password" class="sprited" id="password" name="password" /></td></tr>
			</table>
<div style="text-align:right;">
			<input class="loginbutton" type="submit" id="signin" value="Log In" />
			<input class="loginbutton" type="reset" id="cancel" name="cancel" value="Clear" />
</div>
		</form>
	</body>
</html>'''

#<?xml version="1.0" encoding="UTF-8"?>
#<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
#<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja" lang="ja">

html_main = '''<!doctype html>
<html>
	<head>
		<title>アルゴリズムとデータ構造  - try with konoha</title>
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
		<meta http-equiv="Content-Script-Type" content="text/javascript" />
		<script type="text/javascript" src="../aspen/js/jquery-1.6.1.min.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.cookie.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.lightbox_me.js"></script>
		<script type="text/javascript" src="../aspen/js/codemirror.js"></script>
		<script type="text/javascript" src="../aspen/js/autocompletion.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.lightbox_me.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.periodicalupdater.js"></script>
		<script type="text/javascript" src="../aspen/js/aspen.js"></script>
		<script type="text/javascript" src="../aspen/js/mootools-core-1.3.2-full-compat.js"></script>
		<script type="text/javascript" src="../aspen/mode/konoha/konoha.js"></script>
		<link rel="stylesheet" href="../aspen/css/aspen.css" />
		<link rel="stylesheet" href="../aspen/css/codemirror.css" />
		<link rel="stylesheet" href="../aspen/mode/konoha/konoha.css" />
	</head>
	<body>

		<table border="0" summary="header">
			<tr>
				<td width="230">
					<a href="./"><img src="../aspen/konoha_logo.png" width="213" height="54" alt="Konoha" border=0/></a>
				</td>
				<td>
					<div id="info">
						<span class="user">Hello, %s.</span>
						<span class="date">Last login: %s</span><br />
					</div>
					<form id="logoutform" class="inlineform" name="logoutform" action="./aspen.cgi" method="get">
						<input type="hidden" name="method" value="logout" />
						<input class="normalbutton" type="submit" id="logout" value="Log out" />
					</form>
					<form id="newform" class="inlineform" name="newform" action="./aspen.cgi" method="get">
						<input type="hidden" name="method" value="new" />
						<input class="normalbutton" type="submit" id="new" value="New" />
					</form>
					<input type="button" class="fixedbutton" id="eval" name="eval" alt="Run!!" value="Run" />
<!--					<input type="button" class="fixedbutton" id="save" alt="save the current buffer at remote server" value="Save" />
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
		<textarea id="code" name="code" rows="30" cols="80">print "hello, Konoha";</textarea>
	</body>
</html>'''

def main():
	print 'Content-Type: text/html; charset=UTF-8\n'
	cookie = Cookie.SimpleCookie()
	if os.environ.has_key('HTTP_COOKIE'):
		cookie.load(os.environ['HTTP_COOKIE'])
		if cookie.has_key('UID') and cookie.has_key('LOGIN_DATE'):
			print html_main % (cookie['UID'].value, cookie['LOGIN_DATE'].value)
			return;
		else:
			print html_login;
	else:
		print html_login;

if __name__ == '__main__':
	main()
