
/*
 * LayoutManager detemines the tree and node layout
 */

var LayoutManager = new Class ({ 
	rootNodes : null,
	parent_distance : 0,
	neighbor_distance : 0,
	eachDepthWidth : null,
	eachWidthRatio : null,
	initiateDiffX : 100,
	initiateDiffY : 100,
	widthMax : 0,
	
	initialize : function () {
		this.eachDepthWidth = new Array(20);
		this.eachWidthRatio = new Array(20);
		for (var i = 0; i < 20; i++) {
			this.eachDepthWidth[i] = 0;
			this.eachWidthRatio[i] = 0.0;
		}
		this.parent_distance = 150;
		this.neighbor_distance = 180;
	},

	setDepthForChild : function(pNode, depth) {
		pNode.depth = depth;
		ebigram.addNode(pNode);		
		/* if there is a child... */
		var i = 0;
		var ret = depth
		for (i = 0; i < pNode.connections.length; i++) {
			ret = this.setDepthForChild(pNode.connections[i], depth + 1);
		}
		this.eachDepthWidth[depth] += i ;
		if (depth == 0) {
			var w = this.calculateMaxWidth();
//			console.log(this.eachDepthWidth);
//			console.log("maxWidth", this.widthMax);
		}
		return ret;
	},

	calculateMaxWidth : function () {
		var max = 1;
		for (var i = 0; i < this.eachDepthWidth.length; i++) {
			if (this.eachDepthWidth[i] >= max) {
				max = this.eachDepthWidth[i];
			}
		}
		this.widthMax = max;
		var ratio = 0.0;
		var thisWidth = 0;
		for (var i = 0; i < this.eachDepthWidth.length; i++) {
			thisWidth = this.eachDepthWidth[i];
			if (thisWidth > 0) {
				this.eachWidthRatio[i] = 2 / max * thisWidth;
				console.log(this.eachWidthRatio[i]);
			}
		}
	},

	layoutForRootAs : function (rNode, x, y) {
		ebigram.addNode(rNode);
		rNode.setPosition(x + this.initiateDiffX, y + this.initiateDiffY);
		var depth = rNode.depth;
		var child;
		for (var i = 0; i < rNode.connections.length; i++) {
			child = rNode.connections[i];
			this.layoutForRootAs(
				child,
				this.neighbor_distance * i   + 300 * Math.sin(this.eachWidthRatio[child.depth] * Math.PI),
				this.parent_distance * child.depth
			);
//			this.eachWidthIndex[child.depth]++;
			console.log(child.depth, this.eachWidthRatio[child.depth]);
		}
	},
});
