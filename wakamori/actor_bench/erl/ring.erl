-module (ring).
-export ([start/2]).

start(N, M) ->
	statistics(runtime),
	statistics(wall_clock),
	Main_process = self(),
	spawn(fun() -> ring(1, N, M, self(), Main_process) end),
	receive
		ended -> void
	end.

ring(_, N, _, _, _) when(N =< 0)->
	erlang:error(emptyRing);
ring(_, _, M, _, _) when(M =< 0)->
	erlang:error(noMessagesToSend);
ring(N, N, M, First_process, Main_process) ->
	First_process ! {send, main_process, Main_process},
	loop(M, N, N, First_process, Main_process);
ring(I, N, M, First_process, Main_process) ->
	Next_process = spawn(fun() ->
		ring(I+1, N, M, First_process, Main_process) end),
	loop(M, I, N, Next_process, Main_process).

loop(0, N, N, _, Main_process) ->
	Main_process ! ended;
loop(M, I, N, Next_process, Main_process) ->
	receive
		{send, _, _} ->
			Next_process ! {send, I, self()},
			loop(M-1, I, N, Next_process, Main_process)
	end.
