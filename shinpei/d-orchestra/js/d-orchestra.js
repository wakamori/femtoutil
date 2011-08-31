
/* D-orchestra : written by shinpei_NKT@twitter
 */

// its like main();
// --------------------------
// stats
var STAT_NODESIZE = 30;
var SECRET_MODE = false;
var kong; // global variable
var ebigram; // global variable

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

function saveEbigram() {
	localStorage.setItem("nodes", JSON.stringify(ebigram.nodes));
	localStorage.setItem("edges", JSON.stringify(ebigram.edges));	
}

function loadEbigram() {
	ebigram.nodes = eval(JSON.parse(localStorage.getItem("nodes")));
	ebigram.edges = eval(JSON.parse(localStorage.getItem("edges")));	
}

window.addEvent('domready', function() {
	init();
});

function init()
{
	//kong
	kong = new Kong();

	// console
	$('console').addEvent('click', function(){
		if ($('console').getStyle('height') == "380px") {
			$('console').setStyle("top", "830px");
			$('console').setStyle('height', '10px');
			$('console').innerHTML = "↑";
		} else {
			$('console').setStyle("top", "460px");
			$('console').setStyle('height', '380px');
			$('console').innerHTML = "↓";			
		}
	});
	// ebigram
	ebigram = new Ebigram();
	addElementToEbigram();
}

function addElementToEbigram()
{
//	var s1 = new StartEventNode("start", "start");
	var s1 = new GatewayNode("Start", "start");
	s1.setPosition(10,200);

	var e1 = new MiddleEventNode("Start Webserver", "");
	e1.setPosition(180, 200);
	var e2 = new MiddleEventNode("Start Application server", "FaultA");
	e2.setPosition(360, 200);
	var e3 = new MiddleEventNode("Start Database server", "FaultB");
	e3.setPosition(540, 200);
	var s2 = new GatewayNode("End", "end");
	s2.setPosition(760,200);

	s1.connectTo(e1);
	e1.connectTo(e2);
	e2.connectTo(e3);
	e3.connectTo(s2);

/*	var g2 = new OrGate();
	g2.setPosition(260, 340);
	var g3 = new OrGate();
	g3.setPosition(540, 340);
	var g4 = new OrGate()
	g4.setPosition(910, 340);

	e2.connectTo(g2);
	e3.connectTo(g3);
	e4.connectTo(g4);
*/
	/* g2 */
/*	var e5 = new MiddleEventNode("不適切なAPI利用", "WrongAPIEvent");
	e5.setPosition(170, 430);
	var g5 = new AndGate();
	g5.setPosition(330, 430);
	
	g2.connectToA([e5, g5]);
	var a1 = new AnalyzerEventNode("システムコール履歴", "SystemCall");
	a1.setModel('if-then', 'images/ifthen_info.png', 'script/see_wrong_api.k');
	a1.setPosition(170, 520);
	e5.connectTo(a1);
	
	var e6 = new MiddleEventNode("優先度逆転", null);
	e6.setPosition(250, 610);
	var e7 = new MiddleEventNode("優先度継承失敗", null);
	e7.setPosition(430, 610);
	g5.connectToA([e6, e7]);
	
	var a2 = new AnalyzerEventNode("優先度逆転", "PriorityInversion");
	a2.setModel('StateMachine', 'images/stateMachine_info.png', 'script/see_inversion.k');
	a2.setPosition(250, 720);
	var a3 = new AnalyzerEventNode("優先度継承", "PriorityInheritance");
	a3.setPosition(430, 720);
	a3.setModel('StateMachine', 'images/stateMachine_info.png', 'script/see_inherit.k');	
	e6.connectTo(a2);
	e7.connectTo(a3);
*/
	// g3
/*	var e8 = new MiddleEventNode("スケジューリング設計ミス", null);
	e8.setPosition(510, 430);
	var e9 = new MiddleEventNode("メモリリーク", null);
	e9.setPosition(690, 430);
	g3.connectToA([e8, e9]);
	
	var a4 = new AnalyzerEventNode("実行時間差分", "WrongFeasibility");
	a4.setModel('StateMachine', 'images/stateMachine_info.png', null);
	a4.setPosition(510, 520);
	var a5 = new AnalyzerEventNode("メモリ使用量", "MemoryLeak");
	a5.setModel('if-then', 'images/ifthen_info.png', 'script/see_mem_sample.k');	
	a5.setPosition(690, 520);
	e8.connectTo(a4);
	e9.connectTo(a5);
*/
	// g4
/*	var e10 = new MiddleEventNode("不適切なAPI利用", null);
	e10.setPosition(860, 430);
	var e11 = new MiddleEventNode("メモリリーク", null);
	e11.setPosition(1030, 430);
	g4.connectToA([e10, e11]);
	var a6 = new AnalyzerEventNode("システムコール履歴", "SystemCall2");
	a6.setModel('if-then', 'images/ifthen_info.png', 'script/see_wrong_api.k');
	a6.setPosition(860, 520)
	var a7 = new AnalyzerEventNode("メモリ使用量", "MemoryLeak");
	a7.setModel('if-then', 'images/ifthen_info.png', null);
	a7.setPosition(1030, 520);
	e10.connectTo(a6);
	e11.connectTo(a7);
*/
	var lm = new LayoutManager();
	var dw1 = lm.setDepthForChild(s1, 0);
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
	nodes : null,
	edges : null,
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

	spheres: null,
	popup: null,
	graphs : null,
	nodesDepth : 0,
	
	initialize: function() {
		var canvas = $('ebidiagram');
		this.top = canvas.offsetTop;
		this.left = canvas.offsetLeft;
		this.h = canvas.height;
		this.w = canvas.width;
		this.nodes = new Array();
		this.edges = new Array();
		this.spheres = new Array();
		this.popup = new Array();
		this.glow_lines = new Array();
		this.propertybox = new PropertyBox();
		this.graphs = new Array();
		this.ctx = canvas.getContext('2d');
		
		/* events */
		canvas.addEvent('click', function(that){return function(e){ that.clickFunc(e);};}(this));
		canvas.addEvent('dblclick', function(that){return function(e){ that.dblclickFunc(e);};}(this));
		canvas.addEvent('mousedown', function(that){return function(e){that.downFunc(e);};}(this));
		canvas.addEvent('mousemove', function(that){return function(e){that.moveFunc(e);};}(this));
		canvas.addEvent('mouseup', function(that){return function(e){that.upFunc(e);};}(this));
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

	addEdge : function (edge) {
		this.edges.push(edge);
	},
	/*
	connect : function(from, to) {
		from.connections.push(to);
		var edge = new Edge(from, to);
		this.edges.push(edge);
	},
	*/
	draw : function() {
		this.ctx.save();
		this.ctx.clearRect(0, 0, this.w, this.h);
		this.ctx.fillStyle = this.backgroundColor;
		this.ctx.fillRect(0, 0, this.w, this.h);
		this.ctx.scale(this.zoom, this.zoom);
		var node, edge;
		var moveFast = 2.0;
		for (i = 0; i < this.edges.length; i++) {
			edge = this.edges[i];
			edge.draw(this.ctx);
		}
		for(i = 0; i < this.nodes.length; i++) {
			node = this.nodes[i];

			// if its dragged, move
			node.moveToPoint(this.dx * moveFast, this.dy * moveFast);
			node.draw(this.ctx);
		}

		//=========== effect =============//
		if (this.notify != null) {
			this.notify.draw(this.ctx);
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

			var hitNodes = this.collisionDetect(mouseX, mouseY);
			if (hitNodes.length != 0) {
				// make shadow;
				this.selectedNode = hitNodes[0];
				this.selectedNode.shadow = true;
			} else if (this.selectedNode != null) {
				this.selectedNode.shadow = false;
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

	searchSphereByNameAndKill : function(name) {
		var sphere
		for	(var i = 0; i < this.spheres.length; i++) {
			sphere = this.spheres[i];
			if (name == sphere.name) {
				this.spheres.splice(i, 1);
			}
		}
	},

	searchPopupByNameAndKill : function(name) {
		var tex
		for	(var i = 0; i < this.popup.length; i++) {
			tex = this.popup[i];
			if (name == tex.name) {
				this.popup.splice(i, 1);
			}
		}
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
		if (this.propertybox.isOpened == true) {
			this.propertybox.close();
		}

	},
	
	dblclickFunc : function(event) {
		if (this.selectedNode != null) {
			if (this.propertybox.selectedNode == this.selectedNode) {
				this.propertybox.toggle();
			} else {
				this.propertybox.selectedNode = this.selectedNode;
				this.propertybox.toggle();
			}
		}
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



