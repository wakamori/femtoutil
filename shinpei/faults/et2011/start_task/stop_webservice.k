
// suppose restart_apache.k is already included

void stop_webservice() {
	exec("apache2ctl stop");
}
