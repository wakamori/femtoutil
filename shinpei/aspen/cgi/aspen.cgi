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

#cgitb.enable() for debug

method = os.environ['REQUEST_METHOD']
if method == "GET":
	# output page content
	print '''Content-type: text/html

<!DOCTYPE html>
<html lang="ja">
<meta charset="UTF-8">
<script src='../aspen/js/mootools-core-1.3.2-full-compat.js'></script>
<script src='../aspen/js/codemirror.js'></script>
<link rel='stylesheet' href='../aspen/css/codemirror.css'>

<script src='../aspen/mode/konoha/konoha.js'></script>
<link rel='stylesheet' href='../aspen/mode/konoha/konoha.css'>

<script src="../aspen/js/aspen.js"></script>

<head>
<body>
<!-- modified by wakamori -->
<form><textarea id="code" name="code"></textarea></form>
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
	while os.path.exists(filename + '.k'):
		filename = filename + '-' + str(suffix)
		suffix += 1

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
		print '実行結果'
		for line in outlines:
			print line,

	errlines = p.stderr.readlines()
	if len(errlines) > 0:
		print 'エラー'
		for line in errlines:
			print line,

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
