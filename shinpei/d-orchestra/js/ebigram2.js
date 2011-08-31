
// its like main();
// --------------------------
// stats
var STAT_NODESIZE = 30;
var SECRET_MODE = false;
var kong;
var ebigram;

function secret()
{
	if (SECRET_MODE) {
		SECRET_MODE = false;
	} else {
		SECRET_MODE = true;
	}
	ebigram.reset();
	addElementToEbigram();
}

window.addEvent('domready', function() {
	init();
});

function init()
{
	/*
	var ebigramPane = document.getElementById("ebigramPane");
	var canvas = document.createElement("canvas");
	var propertyboxPane = document.getElementById("propertyboxPane");
	propertyboxPane.width = 300;
	canvas.id = "ebidiagram";
	canvas.width = window.innerWidth - propertyboxPane.width;
	propertyboxPane.style.position = "absolute";
	propertyboxPane.style.top = 130 + "px";
	propertyboxPane.style.left = canvas.width + "px";
	canvas.height = window.innerHeight;
	ebigramPane.appendChild(canvas);
*/
	//kong
	kong = new Kong();
	// test
	//var p = new PropertyBoolean($('property_isActive'), "isActive", "false");
	//p = new PropertyString($('property_caption'), "caption", "TestNode");
	// ebigram
	ebigram = new Ebigram();
	addElementToEbigram();
}

function addElementToEbigram()
{
	var a1 = new Analyzer(STAT_NODESIZE / 1.5, 80, 50);
	var a2 = new Analyzer(STAT_NODESIZE / 1.5, 80, 100);
	var a3 = new Analyzer(STAT_NODESIZE / 1.5, 80, 150);
	var a4 = new Analyzer(STAT_NODESIZE / 1.5, 80, 200);
	var a5 = new Analyzer(STAT_NODESIZE / 1.5, 80, 250);
	var a6 = new Analyzer(STAT_NODESIZE / 1.5, 80, 300);
	var a7 = new Analyzer(STAT_NODESIZE / 1.5, 80, 350);

	var t1 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 50);
	var t2 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 100);
	var t3 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 150);
	var t4 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 200);
	var t5 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 250);
	var t6 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 300);
	var t7 = new MonitoringPoint(STAT_NODESIZE / 1.5, 345, 350);
	var t8 = new MonitoringPoint(STAT_NODESIZE / 1.5, 200, 80);
	var t9 = new MonitoringPoint(STAT_NODESIZE / 1.5, 200, 130);
	var t10 = new MonitoringPoint(STAT_NODESIZE / 1.5, 200, 180);
	var t11 = new MonitoringPoint(STAT_NODESIZE / 1.5, 200, 280);

	var e1 = new ErrorPoint(STAT_NODESIZE, 625, 50);
	var e2 = new ErrorPoint(STAT_NODESIZE, 550, 90);
	var e3 = new ErrorPoint(STAT_NODESIZE, 625, 150);
	var e4 = new ErrorPoint(STAT_NODESIZE, 550, 190);
	var e5 = new ErrorPoint(STAT_NODESIZE, 625, 250);
	var e6 = new ErrorPoint(STAT_NODESIZE, 550, 290);
	var e7 = new ErrorPoint(STAT_NODESIZE, 625, 350);
	
	a1.setCaption("System Integration");
	a1.setName("System Integration");
	a2.setCaption("Application Implementation");
	a2.setName("Application Implementation");
	a3.setCaption("Application Design");
	a3.setName("Application Design");
	a4.setCaption("OS Design");
	a4.setName("OS Design");
	a5.setCaption("OS Implementation");
	a5.setName("OS Implementation");
	a6.setName("Hardware Design");
	a6.setCaption("Hardware Design");
	a7.setName("Hardware Implementation");
	a7.setCaption("Hardwae Implementation");

	t1.setName("T1");
	t1.setCaption("Memory Pressure");
	t2.setName("T2");
	t2.setCaption("CPU Pressure");
	t3.setName("T3");
	t3.setCaption("Unexpected Computation Time");
	t4.setName("T4");
	t4.setCaption("Long Critical Section");
	t5.setName("T5");
	t5.setCaption("Ded not support mutex lock");
	t6.setName("T6");
	t6.setCaption("Large Number of high priority tasks");
	t7.setName("T7");
	t7.setCaption("Power frequency change");
	t8.setName("T8");
	t8.setCaption("App Implementation miss(bug)");
	t9.setName("T9");
	t9.setCaption("App Design Miss(without feasibility study RM)");
	t10.setName("T10");
	t10.setCaption("Break the LTL Safety and Liveness");
	t11.setName("T11");
	t11.setCaption("Attack");

	e1.setName("E1");
	e1.setCaption("Decrease Frame rate");
	e2.setName("E2");
	e2.setCaption("Big jitter of Real-time task");
	e3.setName("E3");
	e3.setCaption("Priority Inversion");
	e4.setName("E4");
	e4.setCaption("Scheduling Delay");
	e5.setName("E5");
	e5.setCaption("Unexpected Abort");
	e6.setName("E6");
	e6.setCaption("Clock Down");
	e7.setName("E7");
	e7.setCaption("Network Congestion");

	if (SECRET_MODE) {
		var body = document.getElementsByTagName("body");
		body[0].style.backgroundColor = "#808080";
		ebigram.setBackgroundColor(0, 0, 0, 1);
		var analyzers = [];
		analyzers.push(a1);
		analyzers.push(a2);
		analyzers.push(a3);
		analyzers.push(a4);
		analyzers.push(a5);
		analyzers.push(a6);
		var i = 0;
		for (i = 0; i < analyzers.length; i++) {
			analyzers[i].setColor(70, 80, 90);
		}
		var mon_points = [];
		mon_points.push(t1);
		mon_points.push(t2);
		mon_points.push(t3);
		mon_points.push(t4);
		for (i = 0; i < mon_points.length; i++) {
			mon_points[i].setColor(0x7f, 0xff, 0xd4);
			mon_points[i].convertTo3D();
		}
		var error_points = [];
		error_points.push(e1);
		error_points.push(e2);
		error_points.push(e3);
		error_points.push(e4);
		for (i = 0; i < error_points.length; i++) {
			error_points[i].setColor(0x7f, 0xff, 0xd4);
			error_points[i].convertTo3D();
		}
	} else {
		var body = document.getElementsByTagName("body");
		body[0].style.backgroundColor = "#ffffff";
		ebigram.setBackgroundColor(0xee, 0xee, 0xee, 1);
	}


	ebigram.addNode(a1);
	ebigram.addNode(a2);
	ebigram.addNode(a3);
	ebigram.addNode(a4);
	ebigram.addNode(a5);
	ebigram.addNode(a6);
	ebigram.addNode(a7);
	
	ebigram.addNode(t1);
	ebigram.addNode(t2);
	ebigram.addNode(t3);
	ebigram.addNode(t4);
	ebigram.addNode(t5);
	ebigram.addNode(t6);
	ebigram.addNode(t7);
	ebigram.addNode(t8);
	ebigram.addNode(t9);
	ebigram.addNode(t10);
	ebigram.addNode(t11);
	
	ebigram.addNode(e1);
	ebigram.addNode(e2);
	ebigram.addNode(e3);
	ebigram.addNode(e4);
	ebigram.addNode(e5);
	ebigram.addNode(e6);
	ebigram.addNode(e7);

	
	/*
	ebigram.connectDetermistic(a1, t1);
	ebigram.connectDetermistic(a2, t2);
	ebigram.connectDetermistic(a3, t3);
	ebigram.connectDetermistic(a4, t4);
	ebigram.connectDetermistic(a5, e1);
	ebigram.connectDetermistic(a6, e2);
	*/

	/*	
	ebigram.connectUndetermistic(t1, e1);
	ebigram.connectUndetermistic(t2, e1);
	ebigram.connectUndetermistic(t3, e1);
	ebigram.connectUndetermistic(t3, e2);
	ebigram.connectUndetermistic(t4, e2);
	
	ebigram.connectUndetermistic(e1, e3);
	ebigram.connectUndetermistic(e2, e4);
	*/
}

var Ebigram = new Class(
{
	title: "undefined",
	h : 400,
	w : 800,
	top : 0,
	left : 0,
	backgroundColor : "#eeeeee",
	wall : null,
	nodes : [],
	edges : [],
	isDragging : 0,
	pre_x : 0,
	pre_y : 0,
	dx : 0,
	dy : 0,
	zoom : 1.0,
	selectedNode : null,
	propertybox : null,
	notify: null,
	ctx: null,
	table: null,
	table_name: "",
	spheres: [],
	popup: [],
	glow_lines: [],

	initialize: function() {
		this.wall = $('ebidiagram');
		this.top = this.wall.offsetTop;
		this.left = this.wall.offsetLeft;
		this.h = this.wall.height;
		this.w = this.wall.width;
		this.propertybox = new PropertyBox();
		var canvas = $('ebidiagram');
		this.ctx = canvas.getContext('2d');
		
		/* events */
		$('ebidiagram').addEvent('click', function(that){return function(e){ that.clickFunc(e);};}(this));
		$('ebidiagram').addEvent('dblclick', function(that){return function(e){ that.dblclickFunc(e);};}(this));
		$('ebidiagram').addEvent('mousedown', function(that){return function(e){that.downFunc(e);};}(this));
		$('ebidiagram').addEvent('mousemove', function(that){return function(e){that.moveFunc(e);};}(this));
		$('ebidiagram').addEvent('mouseup', function(that){return function(e){that.upFunc(e);};}(this));
		//$('ebidiagram').addEvent('mousewheel', function(that){return function(e){that.wheelFunc(e);};}(this));
		int = setInterval(function(that){return function(){that.draw();};}(this), 1000 / 30);
		
	},
	
	setBackgroundColor: function(red, green, blue, alpha) {
		this.backgroundColor = "rgba(" + red + "," + green + "," + blue + "," + alpha + ")";
	},
	
	addNode : function(node) {
		this.nodes.push(node);	
	},
	
	addNotify: function(notify) {
		this.notify = notify;
		//notify.start(this.ctx);
		//notify.addStage(this.ctx);
	},
	
	addPopupMessage: function(message) {
		this.popup.push(message);
	},

	addSphere: function(sphere) {
		this.spheres.push(sphere);
	},

	addLine: function(line) {
		this.glow_lines.push(line);
	},

	connectDetermistic : function(from, to) {
		from.connect_list.push(to);
		var edge = new Edge(from, to);
		if (SECRET_MODE) {
			edge.setColor(255, 255, 255);
		}
		this.edges.push(edge);
	},
	
	connectUndetermistic : function(from, to) {
		this.edges.push(new DashEdge(from, to));
	},
	
	draw : function() {
		this.ctx.save();
		this.ctx.clearRect(0, 0, this.w, this.h);
		this.ctx.fillStyle = this.backgroundColor;
		this.ctx.fillRect(0, 0, this.w, this.h);
		this.ctx.scale(this.zoom, this.zoom);
		var node, edge;
		moveParam = 2.0;
		for(i = 0; i < this.nodes.length; i++) {
			node = this.nodes[i];
			// if its dragged, move
			node.moveToPoint(this.dx * moveParam, this.dy * moveParam);
			node.draw(this.ctx);
		}
		for (i = 0; i < this.edges.length; i++) {
			edge = this.edges[i];
			edge.draw(this.ctx);
		}
		//=========== effect =============//
		if (this.notify != null) {
			this.notify.draw(this.ctx);
		}
		if (this.table != null) {
			this.table.draw(this.ctx);
			if (this.table.isSelected) {
				this.table.glowItem(this.table.selectedItem);
			}
		}
		if (this.spheres.length != 0) {
			for (i = 0; i < this.spheres.length; i++) {
				var node = this.searchNodeByName(this.spheres[i].name);
				this.spheres[i].setPosition(node.pos_x, node.pos_y);
				this.spheres[i].draw(this.ctx);
			}
		}
		if (this.popup.length != 0) {
			for (i = 0; i < this.popup.length; i++) {
				var node = this.searchNodeByName(this.popup[i].name);
				this.popup[i].setPosition(node.pos_x, node.pos_y - 50);
				this.popup[i].addStage(this.ctx);
			}
		}
		if (this.glow_lines.length != 0) {
			for (i = 0; i < this.glow_lines.length; i++) {
				this.glow_lines[i].draw(this.ctx);
			}
		}
		//===============================//
		this.ctx.restore();
		this.idx = 0;
		this.dy = 0;
//		this.zoom = 1.0;
	},
	
	reset: function() {
		this.nodes = [];
		this.edges = [];
	},

	downFunc : function(event) {
		this.isDragging = 1;
		this.pre_x = event.page.x;
		this.pre_y = event.page.y;
	},
	
	upFunc : function() {
		this.isDragging = 0;
		this.dx = 0;
		this.dy = 0;
	},
	
	wheelFunc : function(event) {
		var delta = 0;
		console.log(event.wheel);
		delta = event.wheel;
		if (delta < 0) {
			//zoom out
			this.zoom -= 0.01;
		} else {
			//zoom up
			this.zoom += 0.01;
		}
		
	},
	
	moveFunc : function(event) {
		if (this.isDragging == 1) {
			if (this.selectedNode == null) {
				this.dx = this.pre_x - event.page.x;
				this.dy = this.pre_y - event.page.y;
				this.pre_x = event.page.x;
				this.pre_y = event.page.y;
			} else {
				var node = this.selectedNode;
				var dx = this.pre_x - event.page.x;
				var dy = this.pre_y - event.page.y;
				node.moveToPoint(dx, dy, 1.0);
				this.pre_x = event.page.x;
				this.pre_y = event.page.y;
			}
		} else {
			// show point axis
			mouseX = event.page.x - $('ebigramPane').offsetLeft
			mouseY = event.page.y - $('ebigramPane').offsetTop;
			var textdiv = $('textdiv');
			var px, py;
			px = event.page.x - this.offsetLeft;
			py = event.page.y - $('ebigramPane').offsetTop;
			str = "client : (" + (event.page.x -$('ebigramPane').offsetLeft) + ", " + (event.page.y - $('ebigramPane').offsetTop) + ");";
			textdiv.innerHTML = str;
			
			if (this.table != null) {
				var detect_item = this.table.detectCollision(mouseX, mouseY);
				if (detect_item != "") {
					this.table.glowItem(detect_item);
					this.table.isSelected = true;
					this.table.selectedItem = detect_item;
				} else {
					this.table.isSelected = false;
				}
			}

			hitNodes = this.collisionDetect(mouseX, mouseY);
			if (hitNodes.length != 0) {
				// make shadow;
				this.selectedNode = hitNodes[0];
				this.selectedNode.shadow = true;
				if (SECRET_MODE) {
					this.selectedNode.setGlow(true);
				}
			} else if (this.selectedNode != null) {
				this.selectedNode.shadow = false;
				if (SECRET_MODE) {
					this.selectedNode.setGlow(false);
				}
				this.selectedNode = null;
			}
		}		
	},
	
	searchNodeByName : function(name) {
		var node;
		for	(var i = 0; i < this.nodes.length; i++) {
			node = this.nodes[i];
			if (name == node.name) {
				return node;
			}
		}
		return null;
	},
	
	collisionDetect : function(mouseX, mouseY){
		var distance = 0, node;
		var retnode = [];
		var center = [];
		for (var i = 0; i < this.nodes.length; i++) {
			node = this.nodes[i];
			center = node.getCenter();
			if (mouseX - center[0] <= node.size && mouseY - center[1] <= node.size) {
				distance = Math.sqrt(Math.pow(mouseX - center[0], 2.0) + Math.pow(mouseY- center[1], 2.0));
				if (distance <= node.size) {
					retnode.push(node);
				}
			}
		}
		return retnode;
	},
	
	clickFunc: function(event) {
		if (this.table != null && this.table.isSelected == true) {
			switch (this.table.selectedItem) {
			case "status":
				this.propertybox.selectedNode = this.searchNodeByName(this.table_name);
				this.propertybox.toggle();
				break;
			case "script":
				var window_width = window.innerWidth;
				var window_height = window.innerHeight;
				var width = window_width / 2;
				var height = window_height;
				var left = window_width - width;
				var top = 0;
				var attr = "width=" + width + ", height=" + height + ", left=" + left +
					", top=" + top + ", menubar=no, toolbar=no, scrollbars=yes";
				window.open("../js/script/face_fps.k", "face_fps.k", attr);
				//window.open("../js/script/face_fps.k");
				break;
			case "raw_data":
				break;
			default:
				break;
			}
		}
	},
	
	dblclickFunc : function(event) {
		if (this.selectedNode == null) {
			this.addNode(new ErrorPoint(STAT_NODESIZE, mouseX, mouseY))
		} else {
			// connect
			// show property box;
			console.log(this.selectedNode);
			console.log(event);
			if (this.table_name == this.selectedNode.name) {
				this.table = null;
				this.table_name = "";
			} else {
				this.table_name = this.selectedNode.name;
				this.createTable(this.selectedNode.pos_x,
								 this.selectedNode.pos_y);
			}
			//this.propertybox.selectedNode = this.selectedNode;
			//this.propertybox.toggle();
		}
	},
	
	createTable: function(x, y) {
		var item = ["status", "script", "raw_data"];
		this.table = new Table(item);
		this.table.setPosition(x + 10, y - 50);
		this.table.setSize(100, 100);
		this.table.setColor(99, 99, 99);
		this.table.setOpacity(0.6);
		this.table.setGlowColor(0, 255, 255);
		this.table.draw(this.ctx);
	},
	
});


// Browser Demensions
function getBrowserDimensions() {
        var changed = false;
        if (stage[0] != window.screenX) {
                delta[0] = (window.screenX - stage[0]) * 50;
                stage[0] = window.screenX;
                changed = true;
        }
        
        if (stage[1] != window.screenY) {
                delta[1] = (window.screenY - stage[1]) * 50;
                stage[1] = window.screenY;
                changed = true;
        }
        
        if(stage[2] != window.innerWidth) {
                stage[2] = window.innerWidth;
                changed = true;
        }
        
        if (stage[3] != window.innerHeight) {
                stage[3] = window.innerHeight;
                changed = true;
        }
        return changed;
}



