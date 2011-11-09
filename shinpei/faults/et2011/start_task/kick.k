using konoha.actor.*;

web_service_start_actor = actor:192.168.0.201:50000;
web_service_start_actor.evalFile("restart_apache.k");
web_service_start_actor.evalFile("start_webserver.k");

web_service_start_actor.startTask("start_webservice");
web_service_start_actor.eval("""
start_webservice();
""");
web_service_start_actor.endTask();

