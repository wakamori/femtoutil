	
//var STAT_NODECOLOR =  ["#362C2A","#732420","#BF734C","#FAD9A0","#736859"];
var STAT_NODECOLOR = ["#121212","#E6F2DA","#C9F24B","#4D7B85","#BFAC95"]
//var STAT_NODECOLOR = ["#343F40","#736751","#F2D7B6","#BFAC95","#8C3F3F"];
var TYPE_EP = 2;
var TYPE_MP = 3; /* monitoring point */
var TYPE_AN = 4; /* text label */

var EventNode = new Class(
{
	name: "",
	label: null, /* label for display */
	nodeProperty : null,
	size : 0,
	defaultSize : 0,
	pos_x : 0,
	pos_y : 0,
	color : "",
	shadow : false,
	shadowColor : "#222222",
	//lineColor : "#000000",
	lineColor : "#ffffff",
	connections : null,
	red: 0,
	green: 0,
	blue: 0,
	alpha: 1.0,
	sphere: null,
	depth : 0,

	effectProperty : function(){
		this.label.caption = this.nodeProperty.caption;
	},

	moveToPoint : function(dx, dy) {
		this.pos_x -= dx;
		this.pos_y -= dy;
		//this.size *= zoom;
		if (this.label != null) {
			this.label.pos_x -= dx;
			this.label.pos_y -= dy;
		}
	},
	
	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},

	connectTo : function (toNode) {
		this.connections.push(toNode);
		var edge = new Edge(this, toNode);
		ebigram.addEdge(edge);
	},

	connectToA : function(toNodes) {
		var node;
		var edge;
		for (var i = 0; i < toNodes.length; i++) {
			node = toNodes[i];
			this.connections.push(node);
			edge = new Edge(this, node);
			ebigram.addEdge(edge);
		}
	},
	
	setCaption : function(caption) {
		this.label.caption = caption;
		this.nodeProperty.caption = caption;
		if (this.name == "") {
			this.name = caption;
			this.nodeProperty.nodeName = caption;
		}
	},

	getCaption : function () {
		return this.label.caption;

	},
	
	setName : function (name) {
		this.name = name;
		this.nodeProperty.nodeName = name;
	},
	
	setColor: function(red, green, blue) {
		this.red = red;
		this.green = green;
		this.blue = blue;
		if (this.sphere != null) {
			this.sphere.setColor(this.red, this.green, this.blue);
		}
	},

	setPosition : function(x, y) {
		this.pos_x = x;
		this.pos_y = y;
		this.setLabelPosition(x, y);
	},

	setLabelPosition : function (parentX, parentY) { },
	
	connect : function (node) {
		this.connections.push(node);
	},
	
	setOpacity: function(opacity) {
		this.alpha = opacity;
		if (this.sphere != null) {
			this.sphere.setOpacity(this.alpha);
		}
	},

	searchDepth : function() {
		var i;
		if (this.connections.length == 0) {
			return 1;
		}
	},
});

var StartEventNode = new Class({
	Implements: EventNode,
	w : 0,
	h : 0,
	initialize : function (caption, name) {
		this.size = STAT_NODESIZE;
		this.defaultSize = this.size;
		this.w = this.size * 2;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.connections = new Array();
		this.nodeProperty = new NodeProperty(caption, name);
		this.name = name;
		this.label = new TextLabel(caption);
	},

	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX -20;
		this.label.pos_y = parentY -15;
	},

	draw : function(ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#FFFFFF";
		ctx.save();
		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		ctx.arc(this.pos_x, this.pos_y, this.size, 2 * Math.PI, false);
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}

	},
});

var GatewayNode = new Class({
	Implements: EventNode,
	w : 0,
	h : 0,
	initialize : function (caption, name) {
		this.size = STAT_NODESIZE;
		this.defaultSize = this.size;
		this.w = this.size ;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.connections = new Array();
		this.nodeProperty = new NodeProperty(caption, name);
		this.name = name;
		this.label = new TextLabel(caption);
	},

	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX -20;
		this.label.pos_y = parentY -15;
	},

	draw : function(ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#FFFFFF";
		ctx.save();
		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		ctx.moveTo(this.pos_x, this.pos_y - this.h);
		ctx.lineTo(this.pos_x + this.w, this.pos_y);
		ctx.lineTo(this.pos_x, this.pos_y + this.h);
		ctx.lineTo(this.pos_x - this.w, this.pos_y);
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}

	},
});

var MiddleEventNode = new Class (
{
	Implements : EventNode,
	w : 0,
	h : 0,
	initialize : function (caption, name) {
		this.size = STAT_NODESIZE;
		this.defaultSize = this.size;
		this.w = this.size * 2;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.connections = new Array();
		this.nodeProperty = new NodeProperty(caption, name);
		this.name = name;
		this.label = new TextLabel(caption);
	},

	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX - 50;
		this.label.pos_y = parentY - 20;
	},
	draw : function(ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#ffffff";
		ctx.save(); // push Ctx;
		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		ctx.moveTo(this.pos_x - this.w, this.pos_y - this.h);
		ctx.lineTo(this.pos_x + this.w, this.pos_y - this.h);
		ctx.lineTo(this.pos_x + this.w, this.pos_y + this.h);
		ctx.lineTo(this.pos_x - this.w, this.pos_y + this.h);
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}
	},

	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},
	
});

/*
  var UndeveloppedEventNode = new Class({
	Implements : EventNode,
	
	frameCount : 0,
	isAlert : false,
	isActivate : false,
	
	initialize : function(size, x, y) {
		this.size = size * 1.5
		this.defaultSize = size * 1.5;
		this.color = STAT_NODECOLOR[TYPE_AN];
		this.red = 0xbf;
		this.green = 0xac;
		this.blue = 0x95;
		this.pos_x = x;
		this.pos_y = y;
		this.label = new TextLabel("Undev");
		this.label.pos_x = x - 50 ;
		this.label.pos_y = y - 20;
		this.frameCount = 0;
		this.nodeProperty = new NodeProperty("hoge", "helloworld");
	},
	
	draw : function(ctx) {
		ctx.beginPath();
		ctx.fillStyle = this.color;
		ctx.save(); // push Ctx;
		if (this.shadow) {
			ctx.shadowColor = '#444444';
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		} 
		if (this.isActivate) {
			this.size += Math.sin(this.frameCount / 3);
		} else {
			this.size = this.defaultSize;
		}

		if (this.isAlert) {
			ctx.fillStyle = "rgba(" + 255 + "," + 0 + "," + 0 + "," + this.alpha + ")";
			ctx.strokeStyle = "rgba(" + 255 + "," + 0 + "," + 0 + "," + this.alpha + ")";
		} else {
			ctx.fillStyle = "rgba(" + this.red + "," + this.green + "," + this.blue + "," + this.alpha + ")";
			ctx.strokeStyle = "rgba(" + this.red + "," + this.green + "," + this.blue + "," + this.alpha + ")";
		}

		ctx.beginPath();
		ctx.lineWidth = 3;
		ctx.moveTo(this.pos_x, this.pos_y + this.size);
		ctx.lineTo(this.pos_x - this.size,  this.pos_y);
		ctx.lineTo(this.pos_x, this.pos_y - this.size);
		ctx.lineTo(this.pos_x + this.size , this.pos_y);
		ctx.closePath();

		if (this.isActivate) {
			ctx.fill();
		} else {
			ctx.stroke();
			ctx.fillStyle = "#ffffff";
			ctx.fill();

		}
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}
		this.frameCount++;
	},
	
	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},
});
*/

var AnalyzerEventNode = new Class({
	Implements : EventNode,
	w : 0,
	h : 0,
	isAlert : false,
	
	initialize : function (caption, name) {
		this.size = STAT_NODESIZE * 1.3;
		this.defaultSize = this.size;
		this.w = this.size;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.connections = new Array();
		this.label = new TextLabel(caption);
		this.nodeProperty = new NodeProperty(caption, name);
		this.name = name;
	},

	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX - 30;
		this.label.pos_y = parentY - 20;
	},

	setModel : function (mName, mSrc, mCode) {
		this.nodeProperty.setModel(mName, mSrc, mCode);
	},
	
	draw : function (ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#ffffff";
		ctx.save();

		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		var w = this.w;
		var h = this.h;
		ctx.moveTo(this.pos_x - w, this.pos_y);
		ctx.lineTo(this.pos_x - (w / 2), this.pos_y - h);
		ctx.lineTo(this.pos_x + (w / 2), this.pos_y - h);
		ctx.lineTo(this.pos_x + w, this.pos_y);
		ctx.lineTo(this.pos_x + (w / 2), this.pos_y + h);
		ctx.lineTo(this.pos_x - (w / 2), this.pos_y + h);
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}
	},

	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},
});

var AndGate = new Class ({
	Implements : EventNode,
	w : 0,
	h : 0,
	initialize : function () {
		this.size = STAT_NODESIZE;
		this.defaultSize = this.size;
		this.w = this.size * 0.7;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.nodeProperty = new NodeProperty("AND", null);
		this.connections = new Array();
		this.label = new TextLabel("AND");

	},
/*
	initialize : function (size, x, y) {
		this.size = size;
		this.defaultSize = this.size;
		this.w = this.size * 0.7;
		this.h = this.size;
		this.color = STAT_NODECOLOR[TYPE_EP];
		this.pos_x = x;
		this.pos_y = y;
		this.connections = new Array();
		this.label = new TextLabel("AND");
		this.nodeProperty = new NodeProperty("hoge","hello wolrd");
		this.setLabelPosition(x, y);
	},
*/
	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX - 12;
		this.label.pos_y = parentY - 12;
	},
	
	draw : function (ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#ffffff";
		ctx.save();

		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		var w = this.w;
		var h = this.h;
		ctx.moveTo(this.pos_x, this.pos_y - h);
		ctx.lineTo(this.pos_x + w, this.pos_y - (h / 3));
		ctx.lineTo(this.pos_x + w, this.pos_y + h);
		ctx.lineTo(this.pos_x - w, this.pos_y + h);
		ctx.lineTo(this.pos_x - w, this.pos_y - (h / 3));
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}
	},

	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},
});
	

var OrGate = new Class({
	Implements : EventNode,
	w : 0,
	h : 0,

	initialize : function () {
		this.size = STAT_NODESIZE;
		this.defaultSize = this.size;
		this.w = this.size * 0.7;
		this.h = this.size;
		this.pos_x = 0;
		this.pos_y = 0;
		this.nodeProperty = new NodeProperty("OR", null);
		this.connections = new Array();
		this.label = new TextLabel("OR");
	},
/*
  initialize : function (size, x, y) {
		this.size = size;
		this.defaultSize = this.size;
		this.w = this.size * 0.7;
		this.h = this.size;
		this.color = STAT_NODECOLOR[TYPE_EP];
		this.pos_x = x;
		this.pos_y = y;
		this.connections = new Array();
		this.label = new TextLabel("OR");
		this.setLabelPosition(x, y);
	},
	*/

	setLabelPosition : function (parentX, parentY) {
		this.label.pos_x = parentX - 10;
		this.label.pos_y = parentY - 20;
	},

	draw : function (ctx) {
		ctx.beginPath();
		ctx.fillStyle = "#ffffff";
		ctx.save();

		if (this.shadow) {
			ctx.shadowColor = this.shadowColor;
			ctx.shadowOffsetX = 5;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 5;
		}
		ctx.lineColor = "#000000";
		var w = this.w;
		var h = this.h;
		ctx.moveTo(this.pos_x, this.pos_y - h);
		ctx.lineTo(this.pos_x + w, this.pos_y - (h / 3));
		ctx.lineTo(this.pos_x + w, this.pos_y + h);
		ctx.lineTo(this.pos_x, this.pos_y + (h / 3));
		ctx.lineTo(this.pos_x - w, this.pos_y + h);
		ctx.lineTo(this.pos_x - w, this.pos_y - (h / 3));
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
		ctx.restore();
		if (this.label != null) {
			this.label.draw(ctx);
		}
	},

	getCenter : function () {
		return [this.pos_x, this.pos_y];
	},
});

var TextLabel = new Class (
{
	Implements : EventNode,
	color : "#222222",
	fontSize : 14,
	fontStyle : "monaco",
	font : null,
	caption : null,
	
	initialize: function(text) {
		this.font = this.fontSize + "px " + this.fontStyle;
		if (text != null) {
			this.caption = text;
		} else {
			this.caption = 'undefined';
		}
	},
	
	draw : function(ctx) {
		ctx.fillStyle = "rgba(" + this.red + "," + this.green + "," + this.blue + "," + this.alpha + ")";
		//ctx.fillStyle = this.color;
		ctx.font = this.font;
		var lineMax = 120 / 15;
		if (this.caption.length > lineMax) {
			ctx.fillText(this.caption.slice(0, lineMax), this.pos_x, this.pos_y + 20);
			ctx.fillText(this.caption.slice(lineMax, this.caption.length),
						 this.pos_x, this.pos_y + 40);
		} else {
			ctx.fillText(this.caption, this.pos_x, this.pos_y + 20);
		}
	},
});
