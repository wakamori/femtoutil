
// suppose restart_apache.k is already included

void start_webservice() {
	p = new Proc(["/usr/sbin/apache2ctl", "start"]);
	exec("ls");
	//ApacheConf ac = new ApacheConf(CONF_FILE);
	//ac.setMaxClients("mpm_worker_module", 500);
	print "hi";
}
