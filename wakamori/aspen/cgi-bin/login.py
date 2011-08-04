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
import datetime

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

		return dict(urlparse.parse_qsl(content))

	# Ridirect to the provider
	def redirectToProvider(self, oauth_token_str):
		print "Location: %s?oauth_token=%s\n" % (self.authenticate_url,
				oauth_token_str)

	# save request token into cookie
	def saveRequestCookie(self, rtoken):
		cookie = Cookie.SimpleCookie()
		cookie['request_token'] = rtoken['oauth_token']
		cookie['request_token']['path'] = '/'
		cookie['request_token_secret'] = rtoken['oauth_token_secret']
		cookie['request_token_secret']['path'] = '/'
		print cookie

	# Request the access token
	def getAccessToken(self, request_token, request_token_secret):
		field = cgi.FieldStorage()
		if field.getvalue('denied') != None:
			# authentication denied
			self.redirectToIndex()
		token = oauth.Token(request_token, request_token_secret)
		token.set_verifier(field.getvalue('oauth_verifier'))
		client = oauth.Client(self.consumer, token)

		resp, content = client.request(self.access_token_url, 'POST')
		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

		return dict(urlparse.parse_qsl(content))

	#def getAccountInfo(self, access_token, access_token_secret):
	#	token = oauth.Token(access_token, access_token_secret)
	#	client = oauth.Client(self.consumer, token)

	#	resp, content = client.request(self.account_url, 'GET')
	#	if resp['status'] != '200':
	#		raise Exception("Invalid response %s." % resp['status'])

	#	return json.loads(content)

	def logout(self, access_token, access_token_secret):
		token = oauth.Token(access_token, access_token_secret)
		client = oauth.Client(self.consumer, token)

		resp, content = \
		client.request('http://api.twitter.com/1/account/end_session.json',
		'POST')

		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

	def redirectToIndex(self):
		print "Location: %s\n" % self.conf.get('path', 'base')

	def redirectToDoc(self):
		print 'Location: ./doc\n'

def main():
	lm = LoginManager()
	token = lm.getAccessToken(lm.cookie['request_token'].value,
			lm.cookie['request_token_secret'].value)
	#info = lm.getAccountInfo(token['oauth_token'],
	#		token['oauth_token_secret'])

	uid = token['user_id']
	screen_name = token['screen_name']
	astorage = aspendb.AspenStorage()

	if astorage.hasUser(uid) == False:
		# create user
		access_token = token['oauth_token']
		access_token_secret = token['oauth_token_secret']
		astorage.createUserWithToken(uid, screen_name, access_token,
				access_token_secret)

	# renew session
	sid = astorage.renewSession(uid)

	# save sid into cookie
	cookie = Cookie.SimpleCookie()
	cookie['SID'] = sid
	cookie['SID']['path'] = '/'
	cookie['SID']['expires'] = datetime.datetime.now() + \
		datetime.timedelta(days=1)

	# delete access_token and access_token_secret from cookie
	exptime = datetime.datetime.now() + datetime.timedelta(days=-1)
	cookie['request_token'] = ''
	cookie['request_token']['path'] = ''
	cookie['request_token']['expires'] = exptime
	cookie['request_token_secret'] = ''
	cookie['request_token_secret']['path'] = ''
	cookie['request_token_secret']['expires'] = exptime

	print cookie

	lm.redirectToDoc()

if __name__ == '__main__':
	main()
