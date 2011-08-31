/* connection for ape */

var Kong = new Class({
	ws : null,
	port : 9898,
	
	initialize : function(){
		this.ws = new WebSocket("ws://192.168.59.37:" + this.port + "/");
		console.log(this.ws.status);
		this.ws.onopen = function(that){ return function(){that.onOpen();};}(this);
		this.ws.onmessage = function(that){ return function(msg){that.onMessage(msg);};}(this);
		this.ws.onerror = function(error) {	console.log(error);	};
	},
	
	onOpen : function() {
		console.log("ws open!");
	},
	
	onMessage : function(message) {
		console.log(message.data);
		var json = eval("(" + message.data + ")");
		console.log(json);
		switch (json.msg) {
		case "plugged":
			var name = json.name;
			console.log(name);
			if (name != null) {
				console.log("SUCCESS!");
				var node = ebigram.searchNodeByName(name);
				node.isActivate = true;
			}
			break;
		case "alert":
			var name = json.name;
			this.createNotify("alert", name);
			if (name != null) {
				console.log("nodeName: ", name);
				var node = ebigram.searchNodeByName(name); /* this node is analyzer */
				console.log("Alert", node);
				node.isAlert = true;
				this.popupMessage(node, "alert");
				this.lightNode(node);
			}
			break;
		case "recover":
			var name = json.name;
//			this.createNotify("recover", name);
			if (name != null) {
//				console.log("Recover");
				var node = ebigram.searchNodeByName(name);
				node.isAlert = false;
				ebigram.searchSphereByNameAndKill(name);
				ebigram.searchPopupByNameAndKill(name);
			}
			break;
		case "script":
			//TO DO
			break;
		case "association":
			break;
/*			var start_name = json.from;
			var end_name = json.to;
			//var probability = json.prob;
			var start_analyzer_node = ebigram.searchNodeByName(start_name);//analyzer
			var start_node = ebigram.searchNodeByName(start_analyzer_node.connections[0].name);
			var end_analyzer_node = ebigram.searchNodeByName(end_name);//analyzer
			var end_node = ebigram.searchNodeByName(end_analyzer_node.connections[0].name);
			var line = new GlowLine(start_node.pos_x, start_node.pos_y, end_node.pos_x, end_node.pos_y);
			line.setWidth(4);
			line.setColor(255, 0, 0);
			ebigram.addLine(line);
			break;
			*/
		default:
			console.log("default");
			break;
		}
	},
	
	popupMessage: function(node, state) {
		var src = "";
		switch (state) {
		case "alert":
			src = "images/alert_info.png";
			break;
		case "recover":
			src = "images/recover_info.png";
			break;
		default:
			console.log("ERROR: undefined state");
			break;
		}
		var texture = new Texture(src);
		texture.setName(node.name);
		texture.setPosition(node.pos_x, node.pos_y - 50);
		texture.setSize(50, 50);
		ebigram.addPopupMessage(texture);
	},

	lightNode: function(node) {
		var sphere = new Sphere();
		sphere.setName(node.name);
		sphere.setPosition(node.pos_x, node.pos_y);
		sphere.setColor(255, 0, 0);
		sphere.setSize(node.size * 2);
		//sphere.setGlow(true);
		sphere.animateScale(0.1, 0.9);
		sphere.animateOpacity(1, 0.01);
		sphere.setLoop(700);
		ebigram.addSphere(sphere);
	},

	createNotify: function(state, analyzer_name) {
		var notify;
		notify = new Notify();
		notify.setSize(300, 80);
		notify.setPosition(500, 0);
		switch (state) {
		case "alert":
			notify.setState("[ ALERT ]");
			notify.setStateColor("red");
			notify.setTexture("images/blood.png");
			break;
		case "recover":
			notify.setState("[ RECOVER ]");
			notify.setStateColor("green");
			notify.setTexture("images/recover.png");
			break;
		default:
			break;
		}
		if (SECRET_MODE) {
			notify.setColor(255, 255, 255);
			notify.setTextColor("black");
		} else {
			notify.setTextColor("white");
		}
		notify.setText(analyzer_name);
		ebigram.addNotify(notify);
	},
});

/* Message (S->C) */
var KongMessage = new Class({
	msg : "",
	time : 0,
	data : [],
	
	initialize: function(){
		
	},
	
	stringfy : function() {
		JSON.stringfy(this);
	},
});

/* command (C->S) */
var KongCommand = new Class({
	cmd : "",
	data : [],
	
	initialize: function(){
		
	},
	
	stringfy : function() {
		JSON.stringfy(this);
	},
});
