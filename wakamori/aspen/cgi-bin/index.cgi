#!/usr/bin/python
# -*- coding: utf-8 -*-

import cgitb; cgitb.enable()
import login
import os
import Cookie

html_login = '''<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>Aspen - An online KonohaScript editor</title>
		<link rel="stylesheet" href="../aspen/css/aspen.css" />
		<script type="text/javascript" src="../aspen/js/jquery-1.6.1.min.js"></script>
		<script type="text/javascript" src="../aspen/js/jquery.lightbox_me.js"></script>
		<script type="text/javascript" src="../aspen/js/login.js"></script>
	</head>
	<body>
		<form id="loginform" name="loginform" action="./aspen.cgi?method=oauth" method="post">
		<div class="info">Aspenを使用するには，Twitterでの認証が必要です．</div>
		<div style="text-align:right;">
			<input class="loginbutton" type="submit" id="signin" value="認証する" />
		</div>
		</form>
	</body>
</html>'''

html_main = '''<!DOCTYPE html>
<html>
	<head>
		<title>Aspen - An online KonohaScript editor</title>
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
		<script type="text/javascript" src="../aspen/js/codemirror.js"></script>
		<script type="text/javascript" src="../aspen/js/autocompletion.js"></script>
		<script type="text/javascript" src="../aspen/js/mootools-core-1.3.2-full-compat.js"></script>
		<script type="text/javascript" src="../aspen/js/mootools-more-1.3.2.1.js"></script>
		<script type="text/javascript" src="../aspen/js/aspen.js"></script>
		<script type="text/javascript" src="../aspen/mode/konoha/konoha.js"></script>
		<link rel="stylesheet" href="../aspen/css/aspen.css" />
		<link rel="stylesheet" href="../aspen/css/codemirror.css" />
		<link rel="stylesheet" href="../aspen/mode/konoha/konoha.css" />
	</head>
	<body>
		<table role="presentation">
			<tr>
				<td class="logo">
					<a href="./index.cgi"><img class="logo" src="../aspen/konoha_logo.png" alt="Konoha"/></a>
				</td>
				<td>
					<div class="info">
						<span class="user">Hello, %s.</span>
					</div>
					<form id="logoutform" class="inlineform" name="logoutform" action="./aspen.cgi" method="get">
						<input type="hidden" name="method" value="logout" />
						<input class="normalbutton" type="submit" id="logout" value="Log out" />
					</form>
					<!--
					<form id="newform" class="inlineform" name="newform" action="./aspen.cgi" method="get">
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
			このエディタに関するご質問、ご意見は<img src="../aspen/gmail.png" id="mail" alt="wakamori111 at gmail.com"/>までお願いします。
		</div>
	</body>
</html>'''

def main():
	print 'Content-Type: text/html;charset=UTF-8\n'
	cookie = Cookie.SimpleCookie(os.environ['HTTP_COOKIE'])
	if cookie.has_key('access_token') and cookie.has_key('access_token_secret'):
		lm = login.LoginManager()
		print html_main % (lm.getAccountInfo()['name'].encode('utf-8'))
	else:
		print html_oauth

if __name__ == '__main__':
	main()
