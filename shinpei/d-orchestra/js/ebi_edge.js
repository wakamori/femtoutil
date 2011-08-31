/* edge */
var Edge = new Class ({
	
	from : null,
	to : null,
	color : "#222222",
	lineWidth : 3,
	lineCap : "round",
	red: 0,
	green: 0,
	blue: 0,
	alpha: 1.0,

	initialize : function(from, to) {
		this.from = from;
		this.to = to;
	},
	
	draw : function(ctx){
		ctx.beginPath();
		ctx.lineWidth = this.lineWidth;
		ctx.lineCap = this.lineCap;
		var fromPoint = this.from.getCenter();
		var toPoint = this.to.getCenter();
		ctx.moveTo(fromPoint[0], fromPoint[1]);
		ctx.lineTo(toPoint[0], toPoint[1]);
		ctx.strokeStyle = "rgba(" + this.red + "," + this.green + "," + this.blue + "," + this.alpha + ")";
		ctx.stroke();
	},
	
	setColor: function(red, green, blue) {
		this.red = red;
		this.green = green;
		this.blue = blue;
	},

});


var DashEdge = new Class ({
	Implements : Edge,
	
	draw : function(ctx) {
		var fromPoint = this.from.getCenter();
		var toPoint = this.to.getCenter();
		ret = splitLine(fromPoint[0], fromPoint[1], toPoint[0], toPoint[1]);
		if (ret.length != 0) {
			var p = ret[0];
			var i = 0;
			var flipflop = 0;
			while(ret.length - i >= 2) {
				if (flipflop) {
					ctx.beginPath();
					ctx.lineWidth = this.lineWidth;
					ctx.lineCap = this.lineCap;
					ctx.moveTo(p[0], p[1]);
					i++;
					p = ret[i];
					ctx.lineTo(p[0], p[1]);
					ctx.stroke();
					flipflop = 0;
				} else {
					i += 2;
					p = ret[i];
					flipflop = 1;
				}
			}
		}
	},
	
});
