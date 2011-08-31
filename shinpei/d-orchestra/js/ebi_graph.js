var SmallPane = new Class ({

	w : 0,
	h : 0,
	pos_x : 0,
	pos_y : 0,

	initialize : function () {
		this.w = 400;
		this.h = 600;
		this.pos_x = 0;
		this.pos_y = 0;
	},

	draw : function (ctx ) {
		ctx.beginPath();
		ctx.fillStyle = "#ffddee";
		ctx.save();
		ctx.moveTo(this.pos_x, this.pos_y);
		ctx.fillRect(this.pos_x, this.pos_y, 50, 50);
	},

});
						   
	
