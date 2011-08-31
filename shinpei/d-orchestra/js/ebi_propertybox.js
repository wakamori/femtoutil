
var NodeProperty = new Class({
	id : 0,
	nodeName : null,
	domNodeName : null,
	caption : null,
	domCaption : null,
	domModelImage : null,
	modelName : null,
	modelSrc : null,
	modelCode : null,
	
	initialize : function (caption, name) {
		this.id = 0;
		this.nodeName = name;
		this.caption = caption;
		this.modelName = '';
		this.modelSrc = '';
		this.modelCode ='';
	},	
	
	generateHTML : function () {
		var g = new PropertyElement();
		this.domNodeName = g.generateHTML("string", "name", this.nodeName, this.nodeName);
		this.domCaption = g.generateHTML("string", "caption", this.caption, this.nodeName);
		console.log(this.modelName);
		if (this.modelName == undefined) {
			return this.domNodeName + this.domCaption;
		} else {
			var str = "<img ";
			if (this.modelCode != undefined) {
				str += "onclick='javascript:window.open(\"" + this.modelCode + "\", \"analyzer code\", \"width=800, height=800, toolbar=no, menubar=no, scrollbars=yes, location=no, status=yes, resizable=yes\");' ";
			}
			str += "src='" + this.modelSrc + "'>";
			this.domModel = g.generateHTML("image", "model", str, this.nodeName);
			return this.domNodeName + this.domCaption + this.domModel;
		}
	},

	setModel : function (mName, mSrc, mCode) {
		this.modelName = mName;
		this.modelSrc = mSrc;
		this.modelCode = mCode;
	},
	
	generateEditBox : function (dom) {
		var p = new PropertyString(dom, "name", this.nodeName);
		p = new PropertyString(dom, "caption", this.caption);
	},
});

var PropertyElement = new Class ({
	belongsTo : null, /* node */
	dom : null, /* node dom */ 
	pdom : null,
	isEditing : false,
	
	initialize : function (node) {
		this.belongsTo = node;
	},
	//<div class="property_bool" id='property_isActive'><div class="property_bool_key">isActive</div> : <div class="property_bool_value" id="isActive_value">true</div> </div>
	  
	generateHTML : function (type, key, value, belongsTo) {
		var str = "<div class='property_" + type + "' id='NODE_" + belongsTo + "'><div class='property_" + type + "_key'>" + key + "</div> : <div class='property_" + type + "_value' id='" + key + "_value'>" + value + "</div></div>";
		return str;
	},
});

var PropertyBoolean = new Class ({
	Implements : PropertyElement,
	key : "",
	value : false,
	editDom : null,
	
	initialize : function (dom, key, value) {
		if (key != null) {
			this.key = key;
		} else {
			this.key = "undefined";
		}
		
		if ($type(value) == "boolean") {
			this.value = value;
		} else { 
			this.value = false;
		}
		console.log(dom);
		this.dom = dom;
		this.pdom = this.dom.getElement('div[id$=value]');
		this.pdom.addEvent('click', function(that) { return function() { return that.editProperty();};}(this));
		this.isEditing = false;
	},

	editProperty : function () {
		if (this.isEditing == false) {
			this.pdom.set('html', "<input class='property_boolean_editor' value='" + this.value + "'></input>");
			this.editDom = this.pdom.getElement('input');
			this.editDom.focus();
			this.editDom.addEvent('blur', function(that){return function() {return that.blurFunction();};}(this));
			this.isEditing = true;
		}
	},	
	
	blurFunction : function() {
		if (this.editDom.value == "true") {
			this.value = true;
			this.pdom.set('html', this.value.toString());
			this.isEditing = false;
		} else if (this.editDom.value == "false") {
			this.value = false;
			this.pdom.set('html', this.value.toString());
			this.isEditing = false;		
		}
	},
});

var PropertyImage = new Class ({
	Implements : PropertyElement,
	key : '',
	value : false,
	editDom : null,

	initialize : function (dom, key, value) {
		this.dom = dom;
	},
/*
  		var attr = 'width=800, height=800, menubar=no, toolbar=no, scrollbars=yes, location=no, status=no, resizable=yes';
		window.open("../js/script/face_fps.k", "face_fps.k", attr);
*/
});

var PropertyString = new Class({
	Implements : PropertyElement,
	key : "",
	value : false,
	editDom : null,
	
	initialize : function (dom, key, value) {
		if (key != null) {
			this.key = key;
		} else {
			this.key = "undefined";
		}
		
		if (value != null) {
			this.value = value;
		} else { 
			this.value = "none";
		}
		//this.belongsTo = /* get name from dom */
		this.dom = dom;
		var str = this.dom.getElement("div[id^=NODE]").id;
		this.belongsTo = str.slice(5, str.length);
		//console.log(this.belongsTo);
		var searchElement = "div[id=" + key + "_value]";
		this.pdom = this.dom.getElement(searchElement);
		this.pdom.addEvent('click', function(that) { return function() { return that.editProperty();};}(this));
		this.isEditing = false;
	},

	editProperty : function () {
		if (this.isEditing == false) {
			this.pdom.set('html', "<input class='property_string_editor' value='" + this.value + "'></input>");
			this.editDom = this.pdom.getElement('input');
			this.editDom.focus();
			this.editDom.addEvent('blur', function(that){return function() {return that.blurFunction();};}(this));
			this.isEditing = true;
		}
	},	
	
	blurFunction : function() {
		this.value = this.editDom.value;
		this.pdom.set('html', this.value.toString());
		this.isEditing = false;
		var node = ebigram.searchNodeByName(this.belongsTo);
		console.log(this.belongsTo);
//			console.log(node.nodeProperty[this.key]);
		node.nodeProperty[this.key] = this.value.toString();
		node.effectProperty();
		console.log(node.nodeProperty[this.key]);
			
	},
});


var PropertyBox = new Class({
	dom : null,
	selectedNode : null,
	originalWidth : 0,
	w : 0,
	diff : 0.1,
	closure : null,
	timer : null,
	isOpened : true,
		
	initialize : function() {
		this.dom = $('propertybox');
		console.log(this.dom);
		this.w = getComputedStyle($('propertybox')).width.slice(0, 3);
		this.originalWidth = this.w;
		this.isOpened = false;
	},
	
	toggle : function() {
		if (this.isOpened == true) {
			this.close();
		} else {
			this.open();
		}
	},
	
	open: function() {
		var top = this.selectedNode.pos_y - 50;
		var left = this.selectedNode.pos_x + 10;
		this.dom.style.top = top + "px";
		this.dom.style.left = left + "px";
		this.dom.style.display = "block";
		this.dom.set('html', "");
		this.insertProperties();
		this.isOpened = true;
	},

	close : function () {
		if (this.isOpened == true) {
			this.dom.style.display = "none";
			this.isOpened = false;
		}
	},

	insertProperties : function () {
		var pHTML = this.selectedNode.nodeProperty.generateHTML();
		this.dom.set('html', pHTML);
		this.selectedNode.nodeProperty.generateEditBox(this.dom);
	},


	morphClose : function() {
		if (this.w - this.diff * this.diff >= 0) {
			var width = this.w;
			this.w -= this.diff * this.diff;
			this.diff += 0.2;
			this.dom.style.width  = width + "px";
		} else {
			this.dom.style.width = "0px";
			this.w = 0;
			this.diff = 0.1;	
			$clear(this.timer);
			this.timer = null;
		}
	},
	
	morphOpen : function() {
		if (this.w + this.diff * this.diff <= this.originalWidth) {
			var width = this.w;
			this.w += this.diff * this.diff;
			this.diff += 0.2;
			this.dom.style.width  = width + "px";
		} else {
			this.diff = 0.1;
			this.w = this.originalWidth;
			this.dom.style.width = this.originalWidth + "px";
			$clear(this.timer);
			this.timer = null;
//			this.dom.set('html', this.selectedNode.label.caption);
		}
	},

	
	fetchPropertyFromNode : function (node) {
		var i;
		
		
		
	},
	
});


