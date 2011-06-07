#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
  shinpei_NKT, chen_ji, utrhira (C)2011

 version:
  0.0.4 : divided index.cgi and aspen.cgi. (chen_ji)
  0.0.3 : added jQuery and index.html. (chen_ji)
  0.0.2 : show result with textfile. (utr)
  0.0.1 : first commit. (shinpei_NKT)
'''

import aspendb
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
import urllib2
import Cookie

class Alarm(Exception):
	pass

def alarm_handler(signum, frame):
	raise Alarm

class Aspen:

	def __init__(self):
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
		self.asession = None;
		self.astorage = None;
		self.field = cgi.FieldStorage()
		self.method = os.environ['REQUEST_METHOD']
		self.time = datetime.datetime.now()

	def saveCookie(self, uid, sid):
		exptime = self.time + datetime.timedelta(minutes=30)
		exptxt = exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT')
		self.cookie['UID'] = str(uid)
		self.cookie['UID']['expires'] = exptxt
		self.cookie['UID']['path'] = '/'
		self.cookie['SID'] = str(sid)
		self.cookie['SID']['expires'] = exptxt
		self.cookie['SID']['path'] = '/'
		self.cookie['LOGIN_DATE'] = self.time.strftime('%x,%X')
		self.cookie['LOGIN_DATE']['expires'] = exptxt
		self.cookie['LOGIN_DATE']['path'] = '/'
		print self.cookie

	def login(self):
		username = self.field.getvalue('username')
		self.asession = aspendb.AspenSession(username, self.field.getvalue('password'))
		self.astorage = aspendb.AspenStorage()
		if (self.astorage.authenticate(self.asession)):
			# Authentication successed.
			self.saveCookie(username, self.asession.getSID())
			print 'Location: ../aspen/\n'
		else:
			print 'Content-Type: text/html\n'
			print 'Authentication Failed.'

	def isSignal(self, r, sig):
		if os.path.isfile('/etc/debian_version'):
			# debian
			return r == 128 + sig
		else:
			# other linux and mac (ok?)
			return r == -sig

	def evalScript(self):
		print 'Content-Type: text/html\n'

		kscript = self.field.getvalue('kscript')

		# create script dir
		scrdir = 'scripts'

		foldername = scrdir + '/' + self.cookie['UID'].value
		if not os.path.exists(foldername):
			os.makedirs(foldername)

		# settle script filename
		filename = foldername + '/' + 'us_' + self.cookie['SID'].value

		# create script file
		filename = filename + '.k'
		userscript = open(filename, 'w')
		userscript.write(kscript)
		userscript.close()

		# exec konoha as subprocess
		starttime = time.time()
		command = '/usr/local/bin/konoha ' + filename
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

		try:
			while p.poll() == None:
				outfile.write(p.stdout.readline().replace('\n', '<br />'))
				outfile.flush()
			outfile.close()
			errfile = open(errfilename, 'w')
			errfile.write(p.stderr.read().replace('\n', '<br />'))
			errfile.close()
			signal.alarm(0)
		except Alarm:
			# timeout
			p.terminate()
			errfile = open(errfilename, 'a')
			errfile.write('Konoha was terminated because the program was running \
					too long time (more than 3 minutes).')
			errfile.close()

		# check if process was killed with signal
		r = p.wait()
		exetime = float((time.time() - starttime))

		command = 'w'
		p = subprocess.Popen(command, shell=True,
				stdin=subprocess.PIPE, stdout=subprocess.PIPE,
				stderr=subprocess.PIPE, close_fds=True)

		loadline = p.stdout.readlines()[0]
		load = loadline[loadline.index('load'):-1]

		msg = ''

		if r == 0:
			msg = 'Konoha exited normally. <br />'
		elif self.isSignal(r, signal.SIGSEGV):
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

		msg += '[time: %s, %s]' % (str(exetime)[0:5], load)

		# return values as a json object
		print json.dumps([
			{'key': 'stdout', 'value': open(outfilename, 'r').read()},
			{'key': 'stderr', 'value': open(errfilename, 'r').read()},
			{'key': 'message', 'value': msg}
		])

	def logout(self):
		exptime = self.time + datetime.timedelta(days=-1)
		exptxt = exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT')
		self.cookie['UID']['expires'] = exptxt
		self.cookie['UID']['path'] = '/'
		self.cookie['SID']['expires'] = exptxt
		self.cookie['SID']['path'] = '/'
		self.cookie['LOGIN_DATE']['expires'] = exptxt
		self.cookie['LOGIN_DATE']['path'] = '/'
		print self.cookie
		print 'Location: ../aspen/\n'

	def new(self):
		username = self.cookie['UID'].value
		sid = self.cookie['SID'].value
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSID(username, sid)
		if not self.asession == None:
			self.saveCookie(username, self.asession.getSID())
		print 'Location: ../aspen/\n'

	def run(self):
		if self.method == 'POST':
			mtype = self.field.getvalue('method')
			if mtype == 'login':
				self.login()
			elif mtype == 'eval':
				self.evalScript()
			elif mtype == 'logout':
				self.logout()
			elif mtype == 'new':
				self.new()
			else:
				print 'no such method'

def main():
	a = Aspen()
	a.run()

if __name__ == '__main__':
	main()
