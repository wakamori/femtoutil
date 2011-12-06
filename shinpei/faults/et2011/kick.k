using konoha.actor.*;

int main(String[] args)
{
	oom_level_decreaser = actor:192.168.0.203:50010;
	oom_level_decreaser.startTask("oom_level_decreaser");
	oom_level_decreaser.evalFile("proc.k");
	oom_level_decreaser.evalFile("apache_search.k");
	oom_level_decreaser.eval("""
apache_search();
""");
	oom_level_decreaser.endTask();
	
	memstress_actor = actor:192.168.0.203:50011;
	memstress_actor.startTask("memstress");
	memstress_actor.evalFile("memstress.k");
	memstress_actor.eval("""
invoke_memstress();
""");
	memstress_actor.endTask();
	return 0;

}
