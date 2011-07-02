#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
   chen_ji, shinpei_NKT, utrhira (C)2011

 version:
  0.0.6 : implemented bugreporter (chen_ji)
  0.0.5 : added git-like history (shinpei_NKT)
  0.0.4 : divided index.cgi and aspen.cgi. (chen_ji)
  0.0.3 : added jQuery and index.html. (chen_ji)
  0.0.2 : show result with textfile. (utr)
  0.0.1 : first commit. (shinpei_NKT)
'''

import aspendb
import bugreporter
import cgi
import cgitb; cgitb.enable()
import datetime
import json
import os
import shutil
import signal
import subprocess
import sys
import time
import Cookie

class Alarm(Exception):
	pass

def alarm_handler(signum, frame):
	raise Alarm

class Aspen:

	konohapath = 'path/to/konoha'
	gitpath = 'path/to/.git'

	def __init__(self):
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
		self.field = cgi.FieldStorage()
		self.time = datetime.datetime.now()
		self.method = os.environ['REQUEST_METHOD']

	def printErrorHTML(self, errmsg):
		print 'Content-Type: text/html\n'
		print '''<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>アルゴリズムとデータ構造  - try with konoha </title>
	</head>
		<h1>%s</h1>
	<body>
	</body>
</html>''' % errmsg

	def printText(self, text):
		print 'Content-Type: text/plain\n'
		sys.stdout.write(text)

	def saveCookie(self, uid=None, sid=None, exp=None):
		if exp == None:
			exptime = self.time + datetime.timedelta(minutes=30)
			exp = exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT')
		if uid != None:
			self.cookie['UID'] = str(uid)
			self.cookie['UID']['expires'] = exp
			self.cookie['UID']['path'] = '/'
		if sid != None:
			self.cookie['SID'] = str(sid)
			self.cookie['SID']['expires'] = exp
			self.cookie['SID']['path'] = '/'
		self.cookie['LOGIN_DATE'] = self.time.strftime('%x,%X')
		self.cookie['LOGIN_DATE']['expires'] = exp
		self.cookie['LOGIN_DATE']['path'] = '/'
		print self.cookie

	def login(self):
		username = self.field.getvalue('username')
		self.asession = aspendb.AspenSession(username, self.field.getvalue('password'))
		self.astorage = aspendb.AspenStorage()
		if (self.astorage.authenticate(self.asession)):
			# Authentication successed.
			self.saveCookie(uid=username, sid=self.asession.getSID())
			print 'Location: ./index.cgi\n'
		else:
			self.printErrorHTML('Failed to login.')

	def authWithSID(self):
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSID(
				self.cookie['UID'].value,
				self.cookie['SID'].value)
		if self.asession == None:
			self.printErrorHTML('Failed to authenticate.')

	def authWithSIDAndRenewSession(self):
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSIDAndRenewSession(
				self.cookie['UID'].value,
				self.cookie['SID'].value)
		if self.asession == None:
			self.printErrorHTML('Failed to authenticate and renew.')

	def isSignal(self, r, sig):
		if os.path.isfile('/etc/debian_version'):
			# debian
			return r == 128 + sig
		else:
			# other linux and mac (ok?)
			return r == -sig

	# get current konoha revision
	def setKonohaRevision(self):
		command = 'svnversion %s' % self.konohapath
		p = subprocess.Popen(command,
				shell=True,
				stdout=subprocess.PIPE,
				close_fds=True)
		self.konoha_rev = p.communicate()[0].rstrip('\n')

	# get current aspen version from git hash
	def setAspenVersion(self):
		command = 'git --git-dir=%s log -1 --format="%%h"' % self.gitpath
		p = subprocess.Popen(command,
				shell=True,
				stdout=subprocess.PIPE,
				close_fds=True)
		self.aspen_ver = p.communicate()[0].rstrip('\n')

	# report a script that causes segv as a bug
	def reportBugs(self, body, result):
		br = bugreporter.BugReporter()
		br.setEnvironments(
				self.konoha_rev,
				self.aspen_ver,
				self.cookie['UID'].value,
				self.time)
		br.reportBugs(body, result)

	# store and evaluate current text
	def evalScript(self):
		self.storeScript()
		self.setKonohaRevision()
		self.setAspenVersion()
		foldername = 'scripts/' + self.cookie['UID'].value
		# copy script file for execution
		filename = foldername + '/' + 'us_' + self.cookie['SID'].value + '.k'
		exefilename = foldername + '/aspen.k'
		shutil.copyfile(filename, exefilename)
		# exec konoha as subprocess
		starttime = time.time()
		command = '/usr/local/bin/konoha ' + exefilename
		p = subprocess.Popen(command, shell=True,
				stdin=subprocess.PIPE, stdout=subprocess.PIPE,
				stderr=subprocess.PIPE, close_fds=True)
		outfilename = filename[0:-1] + 'out'
		errfilename = filename[0:-1] + 'err'
		# output result
		outfile = open(outfilename, 'w')
		errfile = open(errfilename, 'w')
		# set timeout
		signal.signal(signal.SIGALRM, alarm_handler)
		signal.alarm(3 * 60) # 3 minutes
		MAX_SIZE = 1024 * 50 # 50 KB
		try:
			size = 0
			while p.poll() == None:
				line = p.stdout.readline()
				while line:
					size += len(line)
					if size < MAX_SIZE:
						outfile.write(line)
						outfile.flush()
					else:
						raise RuntimeError('too long output')
					line = p.stdout.readline()
			outfile.close()
			errfile = open(errfilename, 'w')
			errfile.write(p.stderr.read())
			errfile.close()
			signal.alarm(0)
		except Alarm:
			# timeout
			p.terminate()
			errfile = open(errfilename, 'a')
			errfile.write('Konoha was terminated because the program was running \
too long time (more than 3 minutes).')
			errfile.close()
		except RuntimeError:
			p.terminate()
			errfile = open(errfilename, 'a')
			errfile.write('Konoha was terminated because the output text is \
too long (over 50 KB).')
			errfile.close()
		# check if process was killed with signal
		r = p.wait()
		exetime = float(time.time() - starttime)
		command = 'w'
		p = subprocess.Popen(command, shell=True,
				stdin=subprocess.PIPE, stdout=subprocess.PIPE,
				stderr=subprocess.PIPE, close_fds=True)
		loadline = p.stdout.readlines()[0]
		load = loadline[loadline.index('load'):-1]
		msg = ''
		if r == 0:
			#msg = 'Konoha exited normally. <br />'
			pass
		elif self.isSignal(r, signal.SIGSEGV):
			self.reportBugs(open(exefilename, 'r').read(),
					'[stdout]\n' + open(outfilename, 'r').read() +
					'\n[stderr]\n' + open(errfilename, 'r').read())
			errfile = open(errfilename, 'a')
			errfile.write('Konoha exited unexpectedly. This script will be reported as \
a bug. Sorry.')
			errfile.close()

			# copy script to 'bugs' dir
			bugdir = 'bugs'
			bugfoldername = bugdir + '/' + self.cookie['UID'].value
			if not os.path.exists(bugfoldername):
				os.makedirs(bugfoldername)
			shutil.copy(filename, bugfoldername)
		elif self.isSignal(r, signal.SIGTERM):
			pass
		elif self.isSignal(r, signal.SIGABRT):
			errfile = open(errfilename, 'a')
			errfile.write('Konoha aborted. Sorry.')
			errfile.close()
		else:
			errfile = open(errfilename, 'a')
			errfile.write('Konoha exited unexpectedly with signal: %d. Sorry.' % r)
			errfile.close()
		msg += 'time: %s\n' % str(exetime)[0:5]
		msg += '%s\n' % load
		msg += 'Konoha revision: %s\n' % self.konoha_rev
		msg += 'Aspen version hash: %s' % self.aspen_ver
		# return values as a json object
		print 'Content-Type: application/json;charset=UTF-8\n'
		print json.dumps({'item': [
			{'key': 'stderr', 'value': open(errfilename, 'r').read().decode('utf-8', 'replace')},
			{'key': 'stdout', 'value': open(outfilename, 'r').read().decode('utf-8', 'replace')},
			{'key': 'message', 'value': msg}
		]})

	def logout(self):
		exptime = self.time + datetime.timedelta(days=-1)
		exptxt = exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT')
		self.saveCookie(uid='', sid='', exp=exptxt)
		print 'Location: ./index.cgi\n'

	def replyToRewind(self):
		self.astorage = aspendb.AspenStorage()
		fromsid = self.astorage.rewindSID(self.cookie['SID'].value)
		if fromsid is not '':
			self.saveCookie(sid=fromsid)
			self.printScript()
		else:
			self.printText('oldest')

	def replyToForward(self):
		self.astorage = aspendb.AspenStorage()
		tosid = self.astorage.forwardSID(self.cookie['SID'].value)
		if tosid is not '':
			self.saveCookie(sid=tosid)
			self.printScript()
		else:
			self.printText('latest')

	def new(self):
		self.saveCookie(uid=self.asession.getUID(), sid=self.asession.getSID())
		self.printText('new')

	def name(self):
		fname = self.field.getvalue('filename')
		if not self.asession == None:
			nameSID(self.asession, fname)
		self.printErrorHTML('Failed to name a file.')

	# store current text as a file (named 'us_SID.k')
	def storeScript(self):
		kscript = self.field.getvalue('kscript')
		# create script dir
		scrdir = 'scripts'
		foldername = scrdir + '/' + self.cookie['UID'].value
		if not os.path.exists(foldername):
			os.makedirs(foldername)
		# settle script filename
		filename = foldername + '/us_' + self.cookie['SID'].value
		# create script file
		filename = filename + '.k'
		userscript = open(filename, 'w')
		userscript.write(kscript)
		userscript.close()

	def printScript(self, uid=None, sid=None):
		if uid == None:
			uid = self.cookie['UID'].value
		if sid == None:
			sid = self.cookie['SID'].value
		foldername = 'scripts/' + uid
		filename = foldername + '/us_' + sid + '.k'
		print 'Content-Type: text/plain\n'
		if os.path.isfile(filename):
			sys.stdout.write(open(filename, 'r').read())

	def run(self):
		mtype = self.field.getvalue('method')
		if self.method == 'POST':
			if mtype == 'eval':
				self.authWithSID()
				self.evalScript()
			elif mtype == 'name':
				self.authWithSID()
				self.name()
			elif mtype == 'login':
				self.login()
			elif mtype == 'save':
				self.authWithSID()
				self.store()
			else:
				self.printErrorHTML('No such method in POST.')
		elif self.method == 'GET':
			if mtype == 'new':
				self.authWithSIDAndRenewSession()
				self.new()
			elif mtype == 'rewind':
				self.authWithSID()
				self.replyToRewind()
			elif mtype == 'forward':
				self.authWithSID()
				self.replyToForward()
			elif mtype == 'logout':
				self.logout()
			elif mtype == 'load':
				self.authWithSID()
				self.printScript()
			else:
				self.printErrorHTML('No such method in GET.')
		else:
			self.printErrorHTML('No such method.')

def main():
	a = Aspen()
	a.run()

if __name__ == '__main__':
	main()
