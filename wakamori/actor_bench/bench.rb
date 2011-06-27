require 'benchmark'
require 'fileutils'

actor_count = 3
loop_count = 1000000
precmd = []

# === ruby ===
#dirname = "rb"
#command = "ruby ring.rb %s %s"

# === kilim ===
#dirname = "kil"
#cp = "./classes:./libs/asm-all-2.2.3.jar:./libs/junit.jar"
#precmd.push "javac -cp #{ cp } -g -d ./classes ./bench/kilim/bench/Ring.java"
#precmd.push "java -cp #{ cp } -ea kilim.tools.Weaver -d ./classes -x \"ExInvalid|test\" ./classes"
#command = "java -cp ../kil/classes kilim.bench.Ring -n %s -t %s"

# === erlang ===
#dirname = "erl"
#precmd.push "erlc ring.erl"
#command = "erl -noshell -eval \"ring:start(%s, %s),halt()\""

# === erlang-remote ===
dirname = "erl"
precmd.push "erlc remotering.erl"
#precmd.push "erl -sname foo@localhost &"
#precmd.push "erl -sname bar@localhost &"
#precmd.push "erl -sname hoge@localhost &"
command = "erl -sname bar@localhost -noshell -eval \"remotering:startact1(%s),halt()\""

# === scala ===
#dirname = "scala"
#precmd.push "scalac -d class src1/Ring.scala"
#command = "scala -cp class Ring %s %s a"

# === salsa ===

FileUtils.cd(dirname)

precmd.each do |cmd|
	system(cmd)
end

Benchmark.bm do |x|
	c = Integer(Math::log10(loop_count))
	c.times do |i|
		9.times do |j|
			count = 10 ** i * (j + 1)
			x.report("actor=#{ actor_count },loop=#{ count }:") do
				#system(command % [actor_count, count])
				system(command % count)
			end
		end
	end
end
