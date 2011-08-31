/* utility functions */

function splitLine (x1, y1, x2, y2) {
	var ret = [];
	var gradients = (y2 - y1) / (x2 - x1);
	var intercepts = -gradients * x1 + y1;
	var x_diff = (x2 - x1);
	var small = x2 > x1 ? x1 : x2;
	var big = x2 > x1 ? x2 : x1	;
	
	for (var i = small; i < big; i = i + 5) {
		ret_y = i * gradients + intercepts;
		ret.push([i, ret_y]);
	}

	return ret;	
}
