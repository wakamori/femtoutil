# Ruby Actor Library
# Copyright 2011 Takuma Wakamori <wakamori111 at gmail.com> Twitter:chen_ji
# All rights reserved.

require 'rubygems'
require 'msgpack'
require 'memcache'
require 'socket'
require 'thread'

MEMCACHED_HOST = 'localhost'
TCPSERVER_HOST = '127.0.0.1'
TCPSOCKET_HOST = '127.0.0.1'

class Mailbox < Queue
end

class Message
	attr_accessor :ks_class
	attr_accessor :mtdName
	attr_accessor :msg

	def initialize(ks_class, mtdName, msg)
		@ks_class = ks_class.to_s
		@mtdName = mtdName.to_s
		@msg = msg.to_s
	end

	def convertToKonoha()
		h = Hash.new()
		h["mtdName\000"] = @mtdName + "\000"
		case @msg
		when String
			h["msg\000"] = @msg + "\000"
		when Fixnum
			h["msg\000"] = @msg
		else
			p @msg
			raise
		end
		h["ks:class\000"] = @ks_class + "\000"
		#p h
		return h
	end
end

class Hash
	def convertFromKonoha()
		p self
		h = Hash.new()
		self.each do |key, value|
			case value
			when String
				h[key.split("\000")[0]] = value.split("\000")[0]
			when Fixnum
				h[key.split("\000")[0]] = value
			else
				p value
				raise
			end
		end
		return h
	end
end

class TCPSocket
	def readObject(c)
		msg = MessagePack.unpack(self.read())
		msg = msg.convertFromKonoha()
		return c.new(msg['ks:class'], msg['mtdName'], msg['msg'])
	end

	def writeObject(c)
		#p c.convertToKonoha()
		self.write(MessagePack.pack(c.convertToKonoha()))
		self.flush()
	end
end

module Actor
	def printActorInformation()
		puts '======== <<< Actor Information >>> ========'
		puts "name : #{ @actorName }"
		puts "path : #{ @path }"
		puts "port : #{ @port }"
		puts '==========================================='
	end

	def addMessageToMailbox(co)
		@mailbox.push(co)
	end

	def deliveryThread()
		printActorInformation()
		ts = TCPServer.open(TCPSERVER_HOST, @port)
		while true
			Thread.start(ts.accept) do |s|
				co = s.readObject(Message)
				addMessageToMailbox(co)
				s.close()
			end
		end
	end

	def getScriptPath()
		return File.expand_path(File.dirname($0)) + '/' + $0
	end

	def addMemcached(actorName)
		cache = MemCache.new(MEMCACHED_HOST)
		@port = cache['port'] ? cache['port'].to_i + 1 : 1234
		cache['port'] = @port.to_s
		@path = getScriptPath()
		cache[actorName] = "#{ @path }:#{ @port }"
	end

	def startDeliver()
		t = Thread.new() { deliveryThread() }
	end

	def invokeMethod(msg)
		Object.send(msg.mtdName, msg.msg)
	end

	def startScheduler()
		while true
			unless @mailbox.empty?
				msg = @mailbox.pop()
				invokeMethod(msg)
			end
		end
	end

	def act(actorName)
		@actorName = actorName
		@mailbox = Mailbox.new()
		addMemcached(@actorName)
		startDeliver()
		startScheduler()
	end

	def getActorPathFromMemcached(actorName)
		cache = MemCache.new(MEMCACHED_HOST)
		return cache[actorName]
	end

	def getPathByActorName(actorName)
		path = getActorPathFromMemcached(actorName)
	end

	def printSendMessage(target, path, port, mtdName)
		print "SEND : msg >>> [ name : "
		print "#{ target }, path : #{ path }, port : #{ port }"
		puts ", mtd(msg) : #{ mtdName } ]"
	end

	def sendToActor(target, mtdName, msg)
		actorPath = getPathByActorName(target)
		path, port = actorPath.split(':')
		printSendMessage(target, path, port, mtdName)
		s = TCPSocket.new(TCPSOCKET_HOST, port)
		s.writeObject(Message.new('ConnectionObject', mtdName, msg))
	end
end
