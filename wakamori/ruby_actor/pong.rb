require 'actor'
include Actor

def ping(msg)
	puts "actor2 ping: #{ msg }"
	sendToActor('actor1', 'pong', 'hello world')
end

def hoge(i)
	puts "actor2 hoge: #{ i.to_i + 3 }"
end

act('actor2')
