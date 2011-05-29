#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
提供された情報を表示する
'''

import cgi
import subprocess
import os
import sys
import time

print '''Content-type: text/html

<html>

<script src='../aspen/js/mootools-core-1.3.2-full-compat.js'></script>
<script src='../aspen/js/codemirror.js'></script>
<link rel='stylesheet' href='../aspen/css/codemirror.css'>

<script src='../aspen/mode/konoha/konoha.js'></script>
<link rel='stylesheet' href='../aspen/mode/konoha/konoha.css'>

<head>
<body>
<!--
<script>var myCodeMirror = CodeMirror(document.body)</script>
-->
'''

'''
access time
'''

mon,day,hour,min,sec = time.localtime()[1:6]

scrdir = 'scripts'
foldername = scrdir + '/' + '%02d%02d' % (mon, day)
if not os.path.exists(foldername):
	os.makedirs(foldername)

filename = foldername + '/' + 'us_' + '%02d%02d%02d' % (hour, min, sec)
suffix = 0
while os.path.exists(filename + '.k'):
	filename = filename + '-' + str(suffix)
	suffix += 1

filename = filename + '.k'
userscript = open(filename, 'w')
#userscript.write('print "hi"\nprint "hello"\nthrow new IO!!()') #normal
userscript.write('include') #segv
userscript.close()

command = "/usr/local/bin/konoha " + filename

p = subprocess.Popen(command, shell=True,
		stdin=subprocess.PIPE, stdout=subprocess.PIPE,
		stderr=subprocess.PIPE, close_fds=True)

print '<pre>'

print '実行結果'
for line in p.stdout.readlines():
	print line

print 'エラー'
for line in p.stderr.readlines():
	print line

r = p.wait()

if r == 0:
	print 'プログラムは正常終了しました。'
elif r == -11:
	print 'プログラムは異常終了しました。エラー情報は報告されます。'
	import shutil
	bugdir = 'bugs'
	bugfoldername = bugdir + '/' + '%02d%02d' % (mon, day)
	if not os.path.exists(bugfoldername):
		os.makedirs(bugfoldername)
	shutil.copy(filename, bugfoldername)

print '''</pre>
</body>
</html>'''
