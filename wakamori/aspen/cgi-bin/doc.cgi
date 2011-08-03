#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
 doc.cgi : switch index html
 copyright (c) chen_ji 2011
'''

import Cookie
import os
import login
import aspendb
import datetime
import ConfigParser

html = '''<!DOCTYPE html>
<html lang="ja">
	<head>
		<meta charset="UTF-8">
		<title>Aspen - An online KonohaScript editor</title>

		<!-- for IE compatibility -->
		<!--[if lt IE 9]>
		<script src="http://ie7-js.googlecode.com/svn/version/2.1(beta4)/IE9.js"></script>
		<![endif]-->

		<!-- codemirror -->
		<script src="%(p)sjs/codemirror.js"></script>
		<script src="%(p)sjs/autocompletion.js"></script>
		<link rel="stylesheet" href="%(p)scss/codemirror.css">

		<!-- syntax highlight -->
		<script src="%(p)sjs/konoha.js"></script>
		<link rel="stylesheet" href="%(p)scss/konoha.css">

		<!-- mootools -->
		<script src="%(p)sjs/mootools-core-1.3.2-full-compat.js"></script>
		<script src="%(p)sjs/mootools-more-1.3.2.1.js"></script>

		<!-- aspen -->
		<script src="%(p)sjs/aspen.js"></script>
		<link rel="stylesheet" href="%(p)scss/aspen.css">
	</head>
	<body>
		<!-- debug -->
		<div id="debug">
			%(d)s
		</div>
		<!-- header menu -->
		<header>
			<div id="topmenu">
				<div class="userinfo" id="user"></div>
				<div class="menubutton" id="new">New</div>
				<div class="menubutton" id="eval">Run</div>
				<div class="menubutton" id="sign_out">Sign out</div>
			</div>
		</header>

		<!-- result -->
		<div id="result">
			<span class="message"></span>
		</div>

		<!-- editor body -->
		<textarea id="code" name="code"></textarea>
	</body>
</html>'''

def main():
	cookie = Cookie.SimpleCookie()
	string_cookie = os.environ.get('HTTP_COOKIE')
	cookie.load(string_cookie)

	if cookie.has_key('SID'):
		astorage = aspendb.AspenStorage()
		name = astorage.getScreenName(cookie['SID'].value)
		if name != None:
			conf = ConfigParser.SafeConfigParser()
			conf.read('settings.ini')
			print 'Content-Type: text/html\n'
			print (html % {"p": conf.get('path', 'base'), "d": ("sid=" +
				cookie['SID'].value + ",name=" + name)})
			return
		else:
			# remove cookie and redirect
			cookie['SID']['expires'] = datetime.datetime.now() + \
				datetime.timedelta(days=-1)
			cookie['SID']['path'] = '/'
			print cookie

	lm = login.LoginManager()
	lm.getRequestToken()
	lm.redirectToProvider()

if __name__ == '__main__':
	main()
