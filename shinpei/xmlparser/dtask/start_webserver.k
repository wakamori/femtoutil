#!/usr/local/bin/konoha

//  try to do without DRE
//using konoha.deos.*;
include "dre.k";

void start_webserver(void)
{
	// suppose using DRE API
	AppContainer app = AppContainer.open("webserver");
	if (app.status() == DRE_APP_CONTAINER_STATUS_STOP) {
		app.start();
	} else {
		print "already working";
	}
}
