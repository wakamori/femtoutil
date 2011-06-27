-module(lring).
-export([start/1, loop/1]).
-export([call/2]).

start(M) ->
	Main = self(),
	register(act1, spawn(?MODULE, call, [Main, M])),
	register(act2, spawn(?MODULE, loop, [act1])),
	register(act3, spawn(?MODULE, loop, [act2])),
	act1 ! start,
	receive
		ended -> void
	end.

call(Main, Loopcnt) ->
	receive
		start -> void
	end,
	act3 ! {call, Loopcnt},
	loop(act3),
	receive
		loopend -> void
	end,
	Main ! ended.

loop(act1) ->
	receive
		{call, 0} ->
			act1 ! loopend;
		{call, Loopcnt} ->
			act1 ! {call, Loopcnt-1},
			loop(act1);
		Other ->
			io:format("~p: receive: ~p~n", [node(), Other]),
			exit(receive_unexpected_message)
		after
			3000 ->
				loop(act1)
	end;
loop(Next) ->
	receive
		{call, 0} ->
			Next ! {call, 0};
		{call, Loopcnt} ->
			Next ! {call, Loopcnt},
			loop(Next);
		Other ->
			io:format("~p: receive: ~p~n", [node(), Other]),
			exit(receive_unexpected_message)
		after
			3000 ->
				loop(Next)
	end.

