require 'benchmark'

actor_count = 100
loop_count = 1

Benchmark.bm do |x|
	actor_count.times do |i|
		command = "ruby ring.rb #{ i + 1 } #{ loop_count }"
		x.report("actor=#{ i + 1 },loop=#{ loop_count }:") do
			system(command)
		end
	end
end
