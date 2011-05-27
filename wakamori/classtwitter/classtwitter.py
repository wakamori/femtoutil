#!/c/cygwin/bin/python
# -*- encoding:utf-8 -*-

import ssh
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
PASS = conf.get('secrets', 'pass')

class ClassTwitter:
	def __init__(self, cmd=CMD, cokey=CONSUMER_KEY, cosecret=CONSUMER_SECRET,
			ackey=ACCESS_KEY, acsecret=ACCESS_SECRET, date=DATE):
		self.api = twitter.Api(cokey, cosecret, ackey, acsecret)
		self.s = ssh.Connection(HOST, USER, '', PASS)
		self.cmd = cmd
		self.newstudents = {}
		self.date = date
		f = open('./%s_tweeted.txt' % date, 'a+')
		olds = {}
		for line in f:
			s = line.split(",")
			olds[s[0]] = s[1]
		self.oldstudents = olds
		self.f = f
		self.tweeted = []

	def check(self):
		count = 0
		newlist = self.s.execute(self.cmd)[1:]
		for line in newlist:
			s = line.split(' ')
			key = s[9][:-1]
			if not key in self.newstudents.keys():
				if not key in self.oldstudents.keys():
					count = count + 1
					self.newstudents[key] = s[8]
		print 'newcount=', count, 'stackcount=', len(self.newstudents)

	def run(self, interval=10):
		while True:
			print 'check time=' + time.strftime("%H:%M:%S")
			self.check()
			if len(self.newstudents) > 0:
				self.tweet()
			time.sleep(interval)

	def tweet(self):
		message = '[出席確認済] '
		while len(self.newstudents) > 0:
			s = self.newstudents.popitem()
			message = message + s[0] + 'さん '
			self.tweeted.append(s)
			if len(message) > 140:
				break
		print 'send ' + message.decode('utf-8')
		try:
			self.api.PostUpdate(message.decode('utf-8'))
			self.write()
		except:
			print 'can\'t update'

	def write(self):
		while len(self.tweeted) > 0:
			s = self.tweeted.pop()
			print 'write %s %s' % (s[0], s[1])
			self.f.write(s[0] + ',' + s[1] + '\n')
			self.oldstudents[s[0]] = s[1]
		self.f.flush()

if __name__ == '__main__':
	ct = ClassTwitter()
	ct.run()
