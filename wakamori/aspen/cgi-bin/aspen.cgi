#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 aspen.cgi : small script for invoking konoha
   chen_ji, shinpei_NKT, utrhira (C)2011

 version:
  0.0.5 : added git-like history (shinpei_NKT)
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
import Cookie

class Alarm(Exception):
	pass

def alarm_handler(signum, frame):
	raise Alarm

'''
		if self.mtype == 'login':
			self.login()
		elif self.mtype == 'new':
			self.authWithSIDAndRenewSession()
		else:
			self.authWithSID()
'''


class Aspen:

	konohapath = 'path/to/konoha'
	gitpath = 'path/to/.git'

	def __init__(self):
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
		self.field = cgi.FieldStorage()
		self.mtype = self.field.getvalue('method')
		self.time = datetime.datetime.now()
		self.method = os.environ['REQUEST_METHOD']

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
			print 'Location: ./index.cgi\n'
			return
		print 'Content-Type: text/html\n'
		print 'Failed to login.'

	def authWithSID(self):
		uid = self.cookie['UID'].value
		sid = self.cookie['SID'].value
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSID(uid, sid)
		if self.asession == None:
			print 'Content-Type: text/html\n'
			print 'Failed to authenticate.'

	def authWithSIDAndRenewSession(self):
		uid = self.cookie['UID'].value
		sid = self.cookie['SID'].value
		self.astorage = aspendb.AspenStorage()
		self.asession = self.astorage.authenticateWithSIDAndRenewSession(uid, sid)
		if self.asession == None:
			print 'Content-Type: text/html\n'
			print 'Failed to authenticate and renew.'

	def isSignal(self, r, sig):
		if os.path.isfile('/etc/debian_version'):
			# debian
			return r == 128 + sig
		else:
			# other linux and mac (ok?)
			return r == -sig

	# get current konoha revision
	def getKonohaRevision(self):
		command = 'svnversion %s' % self.konohapath
		p = subprocess.Popen(command,
				shell=True,
				stdout=subprocess.PIPE,
				close_fds=True)
		if os.path.isfile('/etc/debian_version'):
			return p.communicate()[0][0:-2]
		else
			return p.communicate()[0][0:-1]

	# get current aspen version from git hash
	def getAspenVersion(self):
		command = 'git --git-dir=%s log -1 --format="%%h"' % self.gitpath
		p = subprocess.Popen(command,
				shell=True,
				stdout=subprocess.PIPE)
		return p.communicate()[0]

	# save and evaluate current text
	def evalScript(self):
		self.save()
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

		try:
			while p.poll() == None:
				outfile.write(p.stdout.readline())
				outfile.flush()
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
			#msg = 'Konoha exited normally. <br />'
			pass
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

		msg += 'time: %s\n' % str(exetime)[0:5]
		msg += '%s\n' % load
		msg += 'Konoha revision: %s\n' % self.getKonohaRevision()
		msg += 'Aspen version hash: %s' % self.getAspenVersion()

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
		self.cookie['UID']['expires'] = exptxt
		self.cookie['UID']['path'] = '/'
		self.cookie['SID']['expires'] = exptxt
		self.cookie['SID']['path'] = '/'
		self.cookie['LOGIN_DATE']['expires'] = exptxt
		self.cookie['LOGIN_DATE']['path'] = '/'
		print self.cookie
		print 'Location: ./index.cgi\n'

	def replyToRewind(self):
		sid = self.cookie['SID'].value
		self.astorage = aspendb.AspenStorage()
		fromsid = self.astorage.rewindSID(sid)
		if fromsid is not '':
			self.cookie['SID'].value = fromsid
			# create script dir
			scrdir = 'scripts'
			foldername = scrdir + '/' + self.cookie['UID'].value
			if not os.path.exists(foldername):
				os.makedirs(foldername)
			# settle script filename
			filename = foldername + '/' + 'us_' + fromsid
			# create script file
			filename = filename + '.k'
			print self.cookie
			print 'Content-Type: application/json;charset=UTF-8\n'
			print json.dumps({'sid': fromsid})
			return
		print 'Location: ./index.cgi\n'

	def replyToForward(self):
		sid = self.cookie['SID'].value
		self.astorage = aspendb.AspenStorage()
		tosid = self.astorage.forwardSID(sid)
		if tosid is not '':
			self.cookie['SID'].value = tosid
			# create script dir
			scrdir = 'scripts'
			foldername = scrdir + '/' + self.cookie['UID'].value
			if not os.path.exists(foldername):
				os.makedirs(foldername)
			# settle script filename
			filename = foldername + '/' + 'us_' + tosid
			# create script file
			filename = filename + '.k'
			print self.cookie
			print 'Content-Type: application/json;charset=UTF-8\n'
			print json.dumps({'sid': tosid})
		else:
			# there is no tosid. put error.
			print 'Content-Type: application/json;charset=UTF-8\n'
			print json.dumps({'sid': 'none'})
			print self.cookie


	def new(self):
		self.saveCookie(self.asession.getUID(), self.asession.getSID())
		print 'Location: ./index.cgi\n'

	def name(self):
		fname = self.field.getvalue('filename')
		if not self.asession == None:
			nameSID(self.asession, fname)
			print 'Location: ./index.cgi\n'
			return
		print 'Content-Type: text/html\n'
		print 'Failed to name a file.'

	# save current text as a file (named 'us_SID.k')
	def save(self):
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

	def load(self):
		print 'TODO'

	def run(self):
		if self.method == 'POST':
			if self.mtype == 'eval':
				self.authWithSID()
				self.evalScript()
			elif self.mtype == 'name':
				self.authWithSID()
				self.name()
			elif self.mtype == 'rewind':
				self.authWithSID()
				self.replyToRewind()
			elif self.mtype == 'forward':
				self.authWithSID()
				self.replyToForward()
			elif self.mtype == 'login':
				self.login()
			elif self.mtype == 'save':
				self.authWithSID()
				self.save()
				print 'Location: ./index.cgi\n'
			else:
				print 'Content-Type: text/html\n'
				print 'No such method in POST.'
		elif self.method == 'GET':
			if self.mtype == 'new':
				self.authWithSIDAndRenewSession()
				self.new()
			elif self.mtype == 'logout':
				self.logout()
			elif self.mtype == 'load':
				self.authWithSID()
				self.load()
			else:
				print 'Content-Type: text/html\n'
				print 'No such method in GET.'
		else:
			print 'Content-Type: text/html\n'
			print 'No such method.'

def main():
	a = Aspen()
	a.run()

if __name__ == '__main__':
	main()
