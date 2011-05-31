#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
  shinpei_NKT, chen_ji, utrhira (C)2011

 version:
  0.0.2 : show result with textfile. (utr)
  0.0.1 : first commit.(shinpei_NKT)
'''

import cgi
import cgitb
import subprocess
import os
import sys
import time

cgitb.enable()

method = os.environ['REQUEST_METHOD']
if method == "GET":
	# output page content
	print '''Content-type: text/html

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="ja">
<head>
<title>Aspen - An online KonohaScript editor</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Content-Script-Type" content="text/javascript">
<script type="text/javascript" src='../aspen/js/mootools-core-1.3.2-full-compat.js'></script>
<script type="text/javascript" src='../aspen/js/codemirror.js'></script>
<script type="text/javascript" src='../aspen/mode/konoha/konoha.js'></script>
<script type="text/javascript" src="../aspen/js/aspen.js"></script>
<link rel='stylesheet' href='../aspen/css/codemirror.css'>
<link rel='stylesheet' href='../aspen/mode/konoha/konoha.css'>
</head>
<body>
<form action="../cgi-bin/aspen.cgi">
<textarea id="code" name="code" rows="30" cols="80">
</textarea>
</form>
<input type="button" id="eval" name="eval" value="eval"/>
<div id="result"></div>
</body>
</html>'''

elif method == 'POST':
	print '''Content-Type: text/plain
'''
	# get access time
	mon,day,hour,min,sec = time.localtime()[1:6]

	# create script dir
	scrdir = 'scripts'
	foldername = scrdir + '/' + '%02d%02d' % (mon, day)
	if not os.path.exists(foldername):
		os.makedirs(foldername)

	# settle script filename
	filename = foldername + '/' + 'us_' + '%02d%02d%02d' % (hour, min, sec)
	suffix = 0
	while os.path.exists(filename + '-' + str(suffix) + '.k'):
		suffix += 1
	filename = filename + '-' + str(suffix)

	# get kscript from posted content
	kscript = ""
	content_length = int(os.environ['CONTENT_LENGTH'])
	content = cgi.parse_qs(sys.stdin.read(content_length))
	if 'kscript' in content:
		kscript = content['kscript'][0]

	# create script file
	filename = filename + '.k'
	userscript = open(filename, 'w')
	userscript.write(kscript)
	userscript.close()

	# exec konoha as subprocess
	command = "/usr/local/bin/konoha " + filename
	p = subprocess.Popen(command, shell=True,
			stdin=subprocess.PIPE, stdout=subprocess.PIPE,
			stderr=subprocess.PIPE, close_fds=True)

	# output result
	outlines = p.stdout.readlines()
	if len(outlines) > 0:
		for line in outlines:
			print line

	errlines = p.stderr.readlines()
	if len(errlines) > 0:
		for line in errlines:
			print line

	# check if process was killed with signal
	r = p.wait()
	if r == 0:
		print 'プログラムは正常終了しました。'
	elif r == -11:
		print 'プログラムは異常終了しました。エラー情報は報告されます。'
		# copy script to 'bugs' dir
		import shutil
		bugdir = 'bugs'
		bugfoldername = bugdir + '/' + '%02d%02d' % (mon, day)
		if not os.path.exists(bugfoldername):
			os.makedirs(bugfoldername)
		shutil.copy(filename, bugfoldername)
