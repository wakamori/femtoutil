-module(remotering).
-export([startact1/1, startact2/2, startact3/2, loop/3]).
-export([call/3]).

startact1(M) ->
	statistics(runtime),
	statistics(wall_clock),
	Main = self(),
	register(act1, spawn(?MODULE, call, [hoge@localhost, Main, M])),
	rpc:call(foo@localhost, ?MODULE, startact2, [M, act1]),
	rpc:call(hoge@localhost, ?MODULE, startact3, [M, act2]),
	act1 ! start,
	receive
		ended -> void
	end.

startact2(M, Next) ->
	register(act2, spawn(?MODULE, loop, [M, Next, bar@localhost])).
startact3(M, Next) ->
	register(act3, spawn(?MODULE, loop, [M, Next, foo@localhost])).

call(Node, Main, M) ->
	receive
		start -> void
	end,
	{act3, Node} ! call,
	loop(M, act3, Node),
	Main ! ended.

loop(0, _, _) ->
	ok;
loop(M, Next, Node) ->
	receive
		call ->
			{Next, Node} ! call
	end,
	loop(M-1, Next, Node).
