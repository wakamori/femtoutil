#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Aspen OAuth login manager:
  copyright (c) wakamori <wakamori111 at gmail.com> Twitter:@chen_ji
'''

import aspendb
import cgi
import json
import oauth2 as oauth
import os
import urlparse
import urllib
import ConfigParser
import Cookie

class LoginManager:

	request_token_url = 'https://api.twitter.com/oauth/request_token'
	access_token_url = 'https://api.twitter.com/oauth/access_token'
	authenticate_url = 'https://api.twitter.com/oauth/authenticate'
	account_url = 'https://api.twitter.com/1/account/verify_credentials.json'

	def __init__(self):
		self.conf = ConfigParser.SafeConfigParser()
		self.conf.read('settings.ini')
		self.consumer_key = self.conf.get('twitter', 'consumer_key')
		self.consumer_secret = self.conf.get('twitter', 'consumer_secret')
		self.callback_url = self.conf.get('twitter', 'callback_url')
		self.consumer = oauth.Consumer(self.consumer_key, self.consumer_secret)
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))

	# Get a request token
	def getRequestToken(self):
		client = oauth.Client(self.consumer)
		resp, content = None, None
		if self.callback_url == None:
			resp, content = client.request(self.request_token_url, 'GET')
		else:
			resp, content = client.request(self.request_token_url, 'POST',
				body=urllib.urlencode({'oauth_callback': self.callback_url}))
		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

		self.request_token = dict(urlparse.parse_qsl(content))
		cookie = Cookie.SimpleCookie()
		cookie['request_token'] = self.request_token['oauth_token']
		cookie['request_token']['path'] = '/'
		cookie['request_token_secret'] = self.request_token['oauth_token_secret']
		cookie['request_token_secret']['path'] = '/'
		print cookie

	# Ridirect to the provider
	# internal use only
	def redirectToProvider(self):
		print "Location: %s?oauth_token=%s\n" % (self.authenticate_url,
				self.request_token['oauth_token'])

	# Request the access token
	def getAccessToken(self, request_token, request_token_secret):
		field = cgi.FieldStorage()
		token = oauth.Token(request_token, request_token_secret)
		if field.getvalue('denied') != None:
			# authentication denied
			self.redirectToIndex()
		token.set_verifier(field.getvalue('oauth_verifier'))
		client = oauth.Client(self.consumer, token)

		resp, content = client.request(self.access_token_url, 'POST')
		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

		self.access_token = dict(urlparse.parse_qsl(content))
		cookie = Cookie.SimpleCookie()
		cookie['access_token'] = self.access_token['oauth_token']
		cookie['access_token']['path'] = '/'
		cookie['access_token_secret'] = self.access_token['oauth_token_secret']
		cookie['access_token_secret']['path'] = '/'
		print cookie

	def getAccountInfo(self, access_token, access_token_secret):
		token = oauth.Token(access_token, access_token_secret)
		client = oauth.Client(self.consumer, token)

		resp, content = client.request(self.account_url, 'GET')
		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

		return json.loads(content)

	def redirectToIndex(self):
		print "Location: ../\n"

def main():
	lm = LoginManager()
	lm.getAccessToken(lm.cookie['request_token'].value,
			lm.cookie['request_token_secret'].value)
	userid = lm.getAccountInfo(lm.access_token['oauth_token'],
			lm.access_token['oauth_token_secret'])['id_str']
	asession = aspendb.AspenSession(userid, '')
	astorage = aspendb.AspenStorage()
	if astorage.authenticateWithoutPassword(asession):
		cookie = Cookie.SimpleCookie()
		cookie['UID'] = asession.getUID()
		cookie['UID']['path'] = '/'
		cookie['SID'] = asession.getSID()
		cookie['SID']['path'] = '/'
		print cookie
	else:
		raise Exception('Authentication failed')
	lm.redirectToIndex()

if __name__ == '__main__':
	main()
