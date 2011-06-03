#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
  shinpei_NKT, chen_ji, utrhira (C)2011

 version:
  0.0.3 : added jQuery and index.html. (chen_ji)
  0.0.2 : show result with textfile. (utr)
  0.0.1 : first commit. (shinpei_NKT)
'''

import cgi
import cgitb
import json
import os
import shutil
import subprocess
import sys
import time
import aspendb

# enable debug
cgitb.enable()

# get request method
method = os.environ['REQUEST_METHOD']

if method == 'POST':

	# get contents from posted query
	content_length = int(os.environ['CONTENT_LENGTH'])
	contents = cgi.parse_qs(sys.stdin.read(content_length))

	# switch by method type
	if 'method' in content:
		mtype = content['method'][0]
	if mtype == 'login':
		login(content['username'][0], content['password'][0])
	elif mtype == 'eval':
		evalScript(content['kscript'][0])

def login(username, password):
	print 'Content-Type: text/plain\n'
	asession = new AspenSession(username, password)
	astorage = new AspenStorage()
	if (astorage.authenticate(asession)):
		print "Authentication successed."
	print "Authentication Failed."

def evalScript():
	print 'Content-Type: text/plain\n'
	# get access time
	mon, day, hour, min, sec = time.localtime()[1:6]

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
	kscript = ''
	content_length = int(os.environ['CONTENT_LENGTH'])
	content = cgi.parse_qs(sys.stdin.read(content_length))
	if 'kscript' in content:
		kscript = content['kscript'][0]

	# get method type from posted content
	if 'method' in content:
		mtype = content['method'][0]

	# create script file
	filename = filename + '.k'
	userscript = open(filename, 'w')
	userscript.write(kscript)
	userscript.close()

	# exec konoha as subprocess
	command = '/usr/local/bin/konoha ' + filename
	p = subprocess.Popen(command, shell=True,
			stdin=subprocess.PIPE, stdout=subprocess.PIPE,
			stderr=subprocess.PIPE, close_fds=True)

	# output result
	outlines = p.stdout.readlines()
	out = ''
	for line in outlines:
		out = out + line + '<br />'

	errlines = p.stderr.readlines()
	err = ''
	for line in errlines:
		err = err + line + '<br />'

	# check if process was killed with signal
	r = p.wait()
	msg = ''
	if r == 0:
		msg = 'Konoha exited normally.'
	elif r == -11:
		msg = 'Konoha exited unexpectedly. This script will be reported as \
		a bug. Sorry.'
		# copy script to 'bugs' dir
		bugdir = 'bugs'
		bugfoldername = bugdir + '/' + '%02d%02d' % (mon, day)
		if not os.path.exists(bugfoldername):
			os.makedirs(bugfoldername)
		shutil.copy(filename, bugfoldername)

	# return values as a json object
	print json.dumps([
		{'key': 'stdout', 'value': out},
		{'key': 'stderr', 'value': err},
		{'key': 'message', 'value': msg}
		])
