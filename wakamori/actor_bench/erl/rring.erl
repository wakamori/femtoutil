-module(rring).
-export([startact1/1, startact2/1, startact3/1, loop/2]).
-export([call/3]).

startact1(M) ->
	Main = self(),
	register(act1, spawn(?MODULE, call, [baz@localhost, Main, M])),
	rpc:call(foo@localhost, ?MODULE, startact2, [act1]),
	rpc:call(baz@localhost, ?MODULE, startact3, [act2]),
	act1 ! start,
	receive
		ended -> void
	end.

startact2(Next) ->
	register(act2, spawn(?MODULE, loop, [Next, bar@localhost])).
startact3(Next) ->
	register(act3, spawn(?MODULE, loop, [Next, foo@localhost])).

call(Node, Main, Loopcnt) ->
	receive
		start -> void
	end,
	{act3, Node} ! {call, Loopcnt},
	loop(act3, Node),
	receive
		loopend -> void
	end,
	Main ! ended.

loop(Next, bar@localhost) ->
	receive
		{call, 0} ->
			{Next, bar@localhost} ! loopend;
		{call, Loopcnt} ->
			{Next, bar@localhost} ! {call, Loopcnt-1},
			loop(Next, bar@localhost);
		Other ->
			io:format("~p: receive: ~p~n", [node(), Other]),
			exit(receive_unexpected_message)
		after
			3000 ->
				loop(Next, bar@localhost)
	end;
loop(Next, Node) ->
	receive
		{call, 0} ->
			{Next, Node} ! {call, 0};
		{call, Loopcnt} ->
			{Next, Node} ! {call, Loopcnt},
			loop(Next, Node);
		Other ->
			io:format("~p: receive: ~p~n", [node(), Other]),
			exit(receive_unexpected_message)
		after
			3000 ->
				loop(Next, Node)
	end.

