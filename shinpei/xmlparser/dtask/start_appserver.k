#!/usr/local/bin/konoha

//  try to do without DRE
//using konoha.deos.*;
include "dre.k";

void start_appserver()
{
	// suppose using DRE API
	AppContainer app = AppContainer.open("appserver");
	if (app.status() == DRE_APP_CONTAINER_STATUS_STOP) {
		app.start();
	} else {
		print "already working";
	}
}
