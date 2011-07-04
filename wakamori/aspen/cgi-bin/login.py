#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Aspen OAuth login manager:
  copyright (c) wakamori <wakamori111 at gmail.com> Twitter:@chen_ji
'''

import cgi
import json
import oauth2 as oauth
import os
import urlparse
import urllib
import Cookie

class LoginManager:

	consumer_key = 'consumer_key'
	consumer_secret = 'consumer_secret'
	request_token_url = 'https://api.twitter.com/oauth/request_token'
	access_token_url = 'https://api.twitter.com/oauth/access_token'
	authorize_url = 'https://api.twitter.com/oauth/authorize'
	account_url = 'https://api.twitter.com/1/account/verify_credentials.json'

	def __init__(self):
		self.consumer = oauth.Consumer(self.consumer_key, self.consumer_secret)
		self.cookie = Cookie.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))

	# Get a request token
	def getRequestToken(self):
		client = oauth.Client(self.consumer)
		#resp, content = client.request(self.request_token_url, 'GET')
		resp, content = client.request(self.request_token_url, 'POST',
				body=urllib.urlencode({'oauth_callback':'http://localhost/cgi-bin/login.py'}))
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
	def redirectToProvider(self):
		print "Location: %s?oauth_token=%s\n" % (self.authorize_url,
				self.request_token['oauth_token'])

	# Request the access token
	def getAccessToken(self):
		field = cgi.FieldStorage()
		token = oauth.Token(self.cookie['request_token'].value,
				self.cookie['request_token_secret'].value)
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

	def getAccountInfo(self):
		token = oauth.Token(self.cookie['access_token'].value,
				self.cookie['access_token_secret'].value)
		client = oauth.Client(self.consumer, token)

		resp, content = client.request(self.account_url, 'GET')
		if resp['status'] != '200':
			raise Exception("Invalid response %s." % resp['status'])

		return json.loads(content)

	def redirectToIndex(self):
		print 'Location: ./index.cgi\n'

if __name__ == '__main__':
	lm = LoginManager()
	lm.getAccessToken()
	lm.redirectToIndex()
