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
		@ks_class = ks_class
		@mtdName = mtdName
		@msg = msg
	end

	def convertToKonoha()
		h = Hash.new
		h["mtdName\000"] = @mtdName + "\000"
		case @msg
		when String
			h["msg\000"] = @msg + "\000"
		when Fixnum, Array
			h["msg\000"] = @msg
		else
			p @msg
			raise
		end
		h["ks:class\000"] = @ks_class + "\000"
		return h
	end
end

class Hash
	def convertFromKonoha()
		#p self
		h = Hash.new
		self.each do |key, value|
			case value
			when String
				h[key.split("\000")[0]] = value.split("\000")[0]
			when Fixnum, Array
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
		msg = msg.convertFromKonoha
		return c.new(msg['ks:class'], msg['mtdName'], msg['msg'])
	end

	def writeObject(c)
		self.write(MessagePack.pack(c.convertToKonoha))
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
		ts = TCPServer.open(TCPSERVER_HOST, @port)
		while true
			Thread.new(ts.accept) do |s|
				co = s.readObject(Message)
				addMessageToMailbox(co)
				s.close()
			end
		end
	end

	def getScriptDir()
		return File.expand_path(File.dirname($0))
	end

	def getScriptPath()
		return getScriptDir + '/' + $0
	end

	def addMemcached(actorName, port)
		cache = MemCache.new(MEMCACHED_HOST)
		@port = port
		cache['port'] = @port.to_s
		@path = getScriptPath()
		cache[actorName] = "#{ @path }:#{ @port }"
	end

	def startDeliver()
		#printActorInformation()
		Thread.new do
			deliveryThread()
		end
	end

	def invokeMethod(msg)
		#puts "invoke #{ msg.mtdName }(#{ msg.msg })"
		Object.send(msg.mtdName, msg.msg)
	end

	def startScheduler()
		while true
			sleep(0.01)
			unless @mailbox.empty?
				msg = @mailbox.pop
				invokeMethod(msg)
			end
		end
	end

	def act(actorName, port)
		@actorName = actorName
		@mailbox = Mailbox.new()
		addMemcached(@actorName, port)
		startDeliver()
		startScheduler()
	end

	def getActorPathFromMemcached(actorName)
		cache = MemCache.new(MEMCACHED_HOST)
		return cache[actorName]
	end

	def printSendMessage(target, path, port, mtdName, msg)
		print "SEND : msg >>> [ name : #{ target }"
		print ", path : #{ path }, port : #{ port }"
		puts ", mtd(msg) : #{ mtdName }(#{ msg }) ]"
	end

	def sendToActor(target, mtdName, msg)
		actorPath = getActorPathFromMemcached(target)
		path, port = actorPath.split(':')
		#printSendMessage(target, path, port, mtdName, msg)
		s = TCPSocket.new(TCPSOCKET_HOST, port)
		s.writeObject(Message.new('ConnectionObject', mtdName, msg))
		s.close()
	end

	def spawn(actorName, port)
		pid = fork do
			# clild
			act(actorName, port)
		end
		# parent
		while true
			begin
				s = TCPSocket.new(TCPSOCKET_HOST, port)
				break
			rescue
				sleep 0.01
			end
		end
		Process.detach(pid)
	end
end
