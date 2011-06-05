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

import aspendb
import cgi
import cgitb; cgitb.enable()
import datetime
import json
import os
import shutil
import subprocess
import sys
import time
import urllib2
import Cookie

cktxt = 'Set-Cookie: %s=%s;path="/"'
cktxt_exp = 'Set-Cookie: %s=%s;expires=%s'

class Aspen:

	def __init__(self):
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
		self.asession = None;
		self.astorage = None;
		self.field = cgi.FieldStorage()
		self.method = os.environ['REQUEST_METHOD']
		self.time = datetime.datetime.now()

	def saveCookie(self, uid, sid):
		if not self.cookie.has_key('UID'):
			print cktxt % ('UID', str(uid))
		if not self.cookie.has_key('SID'):
			print cktxt % ('SID', str(sid))
		if not self.cookie.has_key('LOGIN_DATE'):
			exptime = self.time + datetime.timedelta(days=30)
			print cktxt % ('LOGIN_DATE',
					exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT'))

	def login(self):
		username = self.field.getvalue('username')
		self.asession = aspendb.AspenSession(username, self.field.getvalue('password'))
		self.astorage = aspendb.AspenStorage()
		if (self.astorage.authenticate(self.asession)):
			# Authentication successed.
			self.saveCookie(username, self.asession.getSID())
			print 'Location: ./index.cgi\n'
		else:
			print 'Authentication Failed.'

	def evalScript(self):
		print 'Content-Type: text/html\n'

		kscript = self.field.getvalue('kscript')

		# create script dir
		scrdir = 'scripts'
		#foldername = scrdir + '/' + self.time.strftime('%m%d')
		#if not os.path.exists(foldername):
		#	os.makedirs(foldername)

		foldername = scrdir + '/' + self.cookie['UID'].value
		if not os.path.exists(foldername):
			os.makedirs(foldername)

		# settle script filename
		filename = foldername + '/' + 'us_' + self.cookie['SID'].value
		#suffix = 0
		#while os.path.exists(filename + '-' + str(suffix) + '.k'):
		#	suffix += 1
		#filename = filename + '-' + str(suffix)

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
		outfile = open(filename[0:-1] + 'out', 'w')
		errfile = open(filename[0:-1] + 'err', 'w')

		while p.poll() == None:
			outfile.write(p.stdout.readline().replace('\n', '<br />'))
			outfile.flush()
		outfile.close()

		while p.poll() == None:
			errfile.write(p.stderr.readline().replace('\n', '<br />'))
			errfile.flush()
		errfile.close()

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
			#bugfoldername = bugdir + '/' + self.time.strftime('%m%d')
			#if not os.path.exists(bugfoldername):
			#	os.makedirs(bugfoldername)

			bugfoldername = bugdir + '/' + self.cookie['UID'].value
			if not os.path.exists(bugfoldername):
				os.makedirs(bugfoldername)

			shutil.copy(filename, bugfoldername)

		# return values as a json object
		print json.dumps([
			{'key': 'stdout', 'value': open(filename[0:-1] + 'out', 'r').read()},
			{'key': 'stderr', 'value': open(filename[0:-1] + 'err', 'r').read()},
			{'key': 'message', 'value': msg}
		])

	def logout(self):
		exptime = self.time + datetime.timedelta(days=-1)
		print cktxt_exp % ('UID', 'logout', exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT'))
		print cktxt_exp % ('SID', 'logout', exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT'))
		print cktxt_exp % ('LOGIN_DATE', 'logout', exptime.strftime('%a, %d-%b-%Y %H:%M:%S GMT'))
		print 'Location: ./index.cgi\n'

	def run(self):
		if self.method == 'POST':
			mtype = self.field.getvalue('method')
			if mtype == 'login':
				self.login()
			elif mtype == 'eval':
				self.evalScript()
			elif mtype == 'logout':
				self.logout()
			else:
				print 'no such method'

def main():
	a = Aspen()
	a.run()

if __name__ == '__main__':
	main()
