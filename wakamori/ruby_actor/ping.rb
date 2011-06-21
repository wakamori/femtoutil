require 'actor'
include Actor

def start(msg)
	puts "actor1 start: #{ msg }"
	sendToActor('actor2', 'ping', 'hello')
	sendToActor('actor2', 'hoge', 4)
end

def pong(msg)
	puts "actor1 pong: #{ msg }"
end

act('actor1')
