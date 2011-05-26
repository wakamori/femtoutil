#!/usr/bin/env python
# -*- encoding:utf-8 -*-

import pexpect
import os
import time
import twitter
import ConfigParser

CONFIG_FILE = 'settings.ini'

conf = ConfigParser.SafeConfigParser()
conf.read(CONFIG_FILE)

CMD = conf.get('options', 'cmd')
DATE = conf.get('options', 'date')
USER = conf.get('commands', 'user')
HOST = conf.get('commands', 'host')
CONSUMER_KEY = conf.get('apis', 'consumer_key')
CONSUMER_SECRET = conf.get('apis', 'consumer_secret')
ACCESS_KEY = conf.get('apis', 'access_key')
ACCESS_SECRET = conf.get('apis', 'access_secret')
PASS = # your pass (ex. os.getenv('CTPASS'))

class Student:
	def __init__(self, name, time):
		self.name = name
		self.time = time
		self.tweeted = False

	def setTweeted(self):
		self.tweeted = True

	def isTweeted(self):
		return self.tweeted

	def getName(self):
		return self.name

	def getTime(self):
		return self.time

class RemoteShell:
	def __init__(self, user=USER, passwd=PASS, host=HOST):
		self.user = user
		self.passwd = passwd
		self.host = host
		
	def run(self, command):
		child = pexpect.spawn('ssh -l %s %s %s' % (self.user, self.host,
			command))
		child.expect('%s@%s\'s password:' % (self.user, self.host))
		child.sendline(self.passwd)
		child.expect(pexpect.EOF)
		return child.before

class ClassTwitter:
	def __init__(self, cmd=CMD, cokey=CONSUMER_KEY, cosecret=CONSUMER_SECRET,
			ackey=ACCESS_KEY, acsecret=ACCESS_SECRET):
		self.api = twitter.Api(cokey, cosecret, ackey, acsecret)
		self.sh = RemoteShell()
		self.cmd = cmd
		self.oldstudents = {}
		self.newstudents = {}

	def check(self, date=DATE):
		newlist = self.sh.run(self.cmd).strip().split('\r\n')[1:]
		for line in newlist:
			s = line.split(' ')
			if not s[2] in self.oldstudents.keys():
				#if s[7] == date:
					self.newstudents[s[2]] = Student(s[2], s[8])

	def run(self, interval=60):
		while True:
			print 'check time=' + time.strftime("%H:%M:%S")
			self.check()
			while len(self.newstudents) > 0:
				tmp = self.newstudents.popitem()
				self.oldstudents[tmp[0]] = tmp[1]
			tweetlist = []
			for old in self.oldstudents.values():
				if not old.isTweeted():
					tweetlist.append(old)
			if len(tweetlist) > 0:
				self.tweet(tweetlist)
			time.sleep(interval)

	def tweet(self, students):
		message = '[出席確認済] '
		for student in students:
			if len(message) > 140:
				break
			message = message + student.getName() + 'さん '
			student.setTweeted()
		print 'send ' + message
		try:
			self.api.PostUpdate(message.decode('utf-8'))
		except:
			print 'can\'t update'

if __name__ == '__main__':
	ct = ClassTwitter()
	ct.run()
