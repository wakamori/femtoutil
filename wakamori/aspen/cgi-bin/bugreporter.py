#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Aspen Bugreporter:
  for reporting bugs via e-mail
  copyright (c) wakamori <wakamori111 at gmail.com> Twitter:@chen_ji
'''

import smtplib
from email.MIMEText import MIMEText
from email.Utils import formatdate

class BugReporter:

	def __init__(self):
		self.subject = '[Aspen BugReporter] Konoha rev:%s bug report'
		self.from_addr = 'from@example.com'
		self.to_addr = 'to@example.com'
		self.s = smtplib.SMTP('mail.example.com', 25)
		self.msg = """ === Konoha bug report by Aspen BugReporter ===
Konoha revision: %s
Aspen version: %s
Writer: %s
Date: %s

Script body:
%s

Execution result:
%s
"""

	def setEnvironments(self, konoha_rev, aspen_ver, writer, date):
		self.konoha_rev = konoha_rev
		self.aspen_ver = aspen_ver
		self.writer = writer
		self.date = date

	def reportBugs(self, body, result):
		msg = MIMEText(self.msg % (self.konoha_rev, self.aspen_ver, self.writer,
			self.date, body, result))
		msg['Subject'] = self.subject % self.konoha_rev
		msg['From'] = self.from_addr
		msg['To'] = self.to_addr
		msg['Date'] = formatdate()
		self.s.sendmail(self.from_addr, [self.to_addr], msg.as_string())

if __name__ == '__main__':
	# for debug
	br = BugReporter()
	br.setEnvironments('123', 'abc', 'hoge', '2011/6/28')
	br.reportBugs('buggy code', 'Segmentation Fault')
