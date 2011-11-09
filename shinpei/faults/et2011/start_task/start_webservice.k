
// suppose restart_apache.k is already included

void start_webservice() {
	exec("apache2ctl start");
	ApacheConf ac = new ApacheConf(CONF_FILE);
	ac.setMaxClients("mpm_worker_module", 500);
}
