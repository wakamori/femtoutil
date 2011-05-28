#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
提供された情報を表示する
'''

import cgi
print """Content-type: text/html

<html>

<script src="../aspen/js/mootools-core-1.3.2-full-compat.js"></script>
<script src="../aspen/js/codemirror.js"></script>
<link rel="stylesheet" href="../aspen/css/codemirror.css">

<script src="../aspen/mode/konoha/konoha.js"></script>
<link rel="stylesheet" href="../aspen/mode/konoha/konoha.css">

<head>
<body>
<script>var myCodeMirror = CodeMirror(document.body)</script>

"""


import subprocess
import os.path

'''
access time
'''

import time
mon,day,hour,min,sec = time.localtime()[1:6]

scrdir = "scripts"
foldername = scrdir + "/" + str(mon) + str(day)
if not os.path.exists(foldername):
    os.makedirs(foldername)


filename = foldername + "/" + 'us_' + str(hour) + str(min) + str(sec)
while os.path.exists(filename):
    filename = filename + '1'

filename = filename + ".k"
userscript = open(filename, 'w')
userscript.write('OUT << "hello world" <<< EOL <<< "goodbye" <<< EOL')
userscript.close()

command = "/usr/local/bin/konoha " + filename
p = subprocess.Popen([command], shell=True,
                     stdin=subprocess.PIPE,stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE,
                     close_fds=True)

for line in p.stdout.readlines():
    print line + "<br>"

for line in p.stderr.readlines():
    print line + "<br>"

print """
</body>
</html>

"""
