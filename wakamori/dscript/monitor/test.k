include "logcache.k"

void main(String[] args)
{
	l = new Logcache("localhost", 11211, "foo");
	Log log = l.readLog();
	while (log.val != null) {
		print log;
		log = l.readLog();
	}
}
