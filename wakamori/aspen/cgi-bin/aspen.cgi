#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
   chen_ji, shinpei_NKT, utrhira (C)2011

 version:
  0.0.7 : twitter authentication implemented (chen_ji)
'''

import aspendb
import bugreporter
import cgi
import cgitb; cgitb.enable()
import datetime
import json
import login
import os
import shutil
import signal
import subprocess
import sys
import time
import ConfigParser
import Cookie

class Alarm(Exception):
	pass

def alarm_handler(signum, frame):
	raise Alarm

class Aspen:

	def __init__(self):
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
		self.field = cgi.FieldStorage()
		self.time = datetime.datetime.now()
		self.method = os.environ['REQUEST_METHOD']
		self.lm = login.LoginManager()
		self.conf = ConfigParser.SafeConfigParser()
		self.conf.read('settings.ini')
		self.konohapath = self.conf.get('path', 'konoha')
		self.gitpath = self.conf.get('path', 'git')

	def printText(self, text):
		print 'Content-Type: text/plain\n'
		sys.stdout.write(text)

	def deleteCookie(self, cookie_keys):
		cookie = Cookie.SimpleCookie()
		exptime = self.time + datetime.timedelta(days=-1)
		exp = exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT')
		for key in cookie_keys:
			cookie[key] = ''
			cookie[key]['expires'] = exp
			cookie[key]['path'] = '/'
		print cookie

	def saveCookie(self, cookies):
		cookie = Cookie.SimpleCookie()
		for c in cookies:
			cookie[c[0]] = c[1]
			cookie[c[0]]['path'] = '/'
		print cookie

	def loginWithTwitter(self):
		self.lm.getRequestToken()
		self.lm.redirectToProvider()

	def authWithSID(self):
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSID(
				self.cookie['UID'].value,
				self.cookie['SID'].value)
		if self.asession == None:
			raise Exception('Failed to authenticate.')

	def authWithSIDAndRenewSession(self):
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSIDAndRenewSession(
				self.cookie['UID'].value,
				self.cookie['SID'].value)
		if self.asession == None:
			raise Exception('Failed to authenticate and renew.')

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
				self.lm.getAccountInfo(self.cookie['access_token'].value,
					self.cookie['access_token_secret'].value)['name'].decode('utf-8'),
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
						raise Exception('too long output')
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
		except Exception:
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

	def logoutWithTwitter(self):
		keys = ['request_token', 'request_token_secret', 'access_token',
		'access_token_secret', 'UID', 'SID']
		self.deleteCookie(keys)
		print "Location: ../\n"

	def replyToRewind(self):
		self.astorage = aspendb.AspenStorage()
		fromsid = self.astorage.rewindSID(self.cookie['SID'].value)
		if fromsid is not '':
			self.saveCookie([('SID', fromsid)])
			self.printScript(sid=fromsid)
		else:
			self.printText('oldest')

	def replyToForward(self):
		self.astorage = aspendb.AspenStorage()
		tosid = self.astorage.forwardSID(self.cookie['SID'].value)
		if tosid is not '':
			self.saveCookie([('SID', tosid)])
			self.printScript(sid=tosid)
		else:
			self.printText('latest')

	def new(self):
		self.saveCookie([('SID', self.asession.getSID())])
		self.printText('new')

	def name(self):
		fname = self.field.getvalue('filename')
		if not self.asession == None:
			nameSID(self.asession, fname)
		raise Exception('Failed to name a file.')

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

	def getScreenName(self):
		name = self.lm.getAccountInfo(
				self.cookie['access_token'].value,
				self.cookie['access_token_secret'].value
				)['name']
		self.printText(name.encode('utf-8'))

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
				self.loginWithTwitter()
			elif mtype == 'save':
				self.authWithSID()
				self.store()
			else:
				raise Exception('No such method in POST.')
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
				self.logoutWithTwitter()
			elif mtype == 'load':
				self.authWithSID()
				self.printScript()
			elif mtype == 'getName':
				self.getScreenName()
			else:
				raise Exception('No such method in GET.')
		else:
			raise Exception('No such method.')

def main():
	a = Aspen()
	a.run()

if __name__ == '__main__':
	main()
