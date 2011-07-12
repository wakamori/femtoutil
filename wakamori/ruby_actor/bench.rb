require 'benchmark'

actor_count = 3
msg_count = 100

Benchmark.bm do |x|
	#actor_count.times do |i|
	#	command = "ruby ring.rb #{ i + 1 } #{ loop_count }"
	#	x.report("actor=#{ i + 1 },loop=#{ loop_count }:") do
	#		system(command)
	#	end
	#end
	msg_count.times do |i|
		command = "ruby ring.rb #{ actor_count } #{ i + 1 }"
		x.report("actor=#{ actor_count },loop=#{ i + 1 }:") do
			system(command)
		end
	end
end
