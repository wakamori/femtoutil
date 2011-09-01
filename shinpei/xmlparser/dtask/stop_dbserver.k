#!/usr/local/bin/konoha
//  try to do without DRE
//using konoha.deos.*;
include "dre.k";

void stop_dbserver(void )
{
	// suppose using DRE API
	AppContainer app = AppContainer.open("dbserver");
	if (app.status() == DRE_APP_CONTAINER_STATUS_RUNNIG) {
		app.stop();
	} else {
		print "its not running";
	}
}
