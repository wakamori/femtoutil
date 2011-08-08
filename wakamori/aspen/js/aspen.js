/*
  aspen.js: Aspen js library

 version:
  0.0.6 : add tabpane
  started by utrhira
  modifed by chen_ji, shinpei_NKT
*/

var Aspen = new Class({
	Implements: Options,

	options: {
		url: "http://localhost/aspen/",
		cgipath: "http://localhost/aspen/cgi-bin/aspen.cgi",
		tabpane: null,
		eval: null,
		new: null,
		open: null,
		signout: null,
		result: null,
		codemirror: new Array()
	},

	isEventAdded: false,

	initialize: function(options) {
		var self = this;
		this.setOptions(options);
		console.log(self.options);

		self.addTab("noname.k", 'print "hello, noname";');

		var myTabPane = new TabPane(self.options.tabpane.get("id"), {
			contentSelector: "div.CodeMirror"
		});

		//// load script from aspen db
		//var loadReq = new Request({
		//	url: self.options.cgipath,
		//	method: "get",
		//	data: {
		//		"method": "load",
		//		"time": new Date().getTime() // for IE
		//	},
		//	onSuccess: function(responseText) {
		//		if (responseText == "") {
		//			editor.setValue("print \"hello, Konoha\";");
		//		} else {
		//			editor.setValue(responseText);
		//		}
		//	}
		//});
		//loadReq.send();

		// set actions
		self.options.eval.addEvent("click", function() {
			if (this.getProperty("disabled") == false) {
				//mySlide.slideIn();
				var tabs = self.options.tabpane.getElements("li.tab");
				console.log(tabs);
				var activeTab = self.options.tabpane.getElement("li.tab.active");
				console.log(activeTab);
				console.log(tabs.indexOf(activeTab));
				var editor = self.options.codemirror[tabs.indexOf(activeTab)];
				console.log(editor);
				var text = editor.getValue();
				console.log(text);
				if (text.length > 0) {
					self.postScript(text);
				}
			}
		});
		self.options.new.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"method": "new",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function() {
					editor.setValue("print \"hello, Konoha\";");
				}
			});
			req.send();
		});
		self.options.tabpane.addEvent("click:relay(.remove)", function(e) {
			if (self.options.codemirror.length <= 1) {
				// don't remove tab but clear it
				self.options.codemirror[0].setValue("");
				this.getParent().set("text", "noname.k");
			} else {
				new Event(e).stop();
				var parent = this.getParent("li.tab");
				var index = self.options.tabpane.getElements("li.tab").indexOf(parent);
				var textarea = self.options.tabpane.getElements(".dummy")[index];
				textarea.destroy();
				self.options.codemirror.splice(index, 1);
				myTabPane.closeTab(index);
			}
		});
		self.options.tabpane.addEvent("click:relay(.tab)", function(e) {
			console.log("relaytab");
			var index = this.getParent().getElements("li.tab").indexOf(this);
			window.fireEvent("resize", index);
			self.options.codemirror[index].refresh();
		});
		self.options.open.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"method": "open",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					Shadowbox.init({
						onFinish: function() {
							self.setActions();
							if (!self.isEventAdded) {
								self.isEventAdded = true;
								self.addLoadEvent();
							}
						}
					});
					Shadowbox.open({
						content: '<div id="filemanager">' + self.getULStringfromArray(JSON.decode(data)["item"]) + "</div>",
						player: "html",
						title: "Open file"
					});
				}
			});
			req.send();
		});
		self.options.signout.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"method": "logout",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					document.location = self.options.url;
				}
			});
			req.send();
		});
	},

	// add new tab
	addTab: function(tabname, content) {
		var self = this;
		var keyEvent = function(editor, key) {
			if (key.type == "keydown" && key.keyCode == 13 && key.shiftKey) {
				if (self.options.eval.getProperty("disabled") == false) {
					//var mySlide = new Fx.Slide("result");
					//mySlide.slideIn();
					var text = editor.getValue();
					console.log("text="+text);
					if (text.length > 0) {
						self.postScript(text);
					}
				}
				key.returnValue = false;
			}
		};
		var tabpane = document.id("tabpane");
		var textarea = new Element("textarea", {
			"class": "dummy"
		});
		tabpane.grab(textarea);
		console.log(tabpane);
		var myeditor = CodeMirror.fromTextArea(textarea, {
			lineNumbers: true,
			matchBrackets: true,
			mode: "text/konoha",
			onKeyEvent: keyEvent
		});
		var tabli = new Element("li", {
			"class": "tab",
			text: tabname
		}).adopt(new Element("span", {
			"class": "remove",
			html: "&times"
		}));
		/*
		tabli.addEvent("click", function() {
			console.log("refresh");
			window.fireEvent("resize", this.getParent().getElements(".tab").indexOf(this));
			myeditor.refresh();
		});
		*/
		tabpane.getElement("ul").grab(tabli);
		myeditor.getWrapperElement().setStyle("display", "none");
		//console.log(myeditor.getValue());
		myeditor.setValue(content);
		//console.log(myeditor.getValue());
		self.options.codemirror.push(myeditor);
		//console.log(self.options.codemirror);
	},

	// set filemanager action
	setActions: function() {
		var self = this;
		var dirs = document.getElements("div.dir");
		for (var i = 0; i < dirs.length; i++) {
			dirs[i].addEvent("click", function() {
				this.getParent().getElement("ul").toggle();
			});
		}
		var files = document.getElements("div.file");
		for (var i = 0; i < files.length; i++) {
			files[i].addEvent("click", function() {
				var parents = this.getParents("li");
				if (parents.length <= 1) {
					document.id("sb-nav-filename").set("value", this.get("text"));
				} else {
					document.id("sb-nav-filename").set("value", this.getParents("li")[1].get("id").substring(5) + "/" + this.get("text"));
				}
				for (var j = 0; j < files.length; j++) {
					if (files[j].get("selected") == true) {
						files[j].set("selected", false);
						files[j].setStyle("background-color", "transparent");
						break;
					}
				}
				this.set("selected", true);
				this.setStyle("background-color", "yellow");
				console.log(files.get("selected"));
			});
		}
	},

	// add fileload event into generated dom objects
	addLoadEvent: function() {
		var self = this;
		document.id("sb-nav-open").addEvent("click", function() {
			if (this.getProperty("disabled") == true) return;
			this.setProperty("disabled", true);
			var filename = document.id("sb-nav-filename").get("value");
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"method": "load",
					"file": filename,
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(text) {
					self.addTab(filename, text);
				}
			});
			req.send();
			Shadowbox.close();
			this.setProperty("disabled", false);
		});
	},


	// escape text
	escapeText: function(text) {
		text = text.replace(/&/g, "&amp;");
		text = text.replace(/</g, "&lt;");
		text = text.replace(/>/g, "&gt;");
		text = text.replace(/"/g, "&quot;");
		text = text.replace(/ /g, "&nbsp;");
		text = text.replace(/\r\n/g, "<br>");
		text = text.replace(/(\n|\r)/g, "<br>");
		return text;
	},

	// get ul element string from array
	getULStringfromArray: function(data) {
		// console.log(data);
		var ret = new Element("ul", {
			html: '<li id="dir_/">/</li>'
		});
		for (var i = 0; i < data.length; i++) {
			if (i == 0) {
				var dirname = data[i][0]; // /
			} else {
				var dirname = data[i][0] + "/"; // /dir1/
			}
			var childdirs = data[i][1]; // dir1, dir2
			var childfiles = data[i][2]; // xxx.k, ...
			var dir = ret.getElementById("dir_" + data[i][0]); // dir_/dir1
			if (i == 0) {
				dir.set("html", dir.get("html") + '<ul id="filelist" class="filelist"></ul>');
			} else {
				dir.set("html", dir.get("html") + '<ul class="filelist" style="display: none;"></ul>');
			}
			// console.log(dir);
			for (var j = 0; j < childdirs.length; j++) {
				dir.getElement("ul").grab(new Element("li", {
					id: "dir_" + dirname + childdirs[j],
					html: '<div class="dir">' + childdirs[j] + "/</div>"
				}));
			}
			for (var j = 0; j < childfiles.length; j++) {
				dir.getElement("ul").grab(new Element("li", {
					html: '<div class="file">' + childfiles[j] + "</div>"
				}));
			}
		}
		// console.log(ret.getElementById("filelist"));
		var tmp = new Element("div");
		tmp.grab(ret.getElementById("filelist"));
		return tmp.get("html");
	},

	// post script and eval
	postScript: function(text) {
		var self = this;
		self.options.result.set("html", "");
		var inputtxt = new Element("span", {
			"class": "message",
			html: "Running ..."
		});
		inputtxt.inject(self.options.result);
		self.options.eval.setProperty("disabled", true);
		var periodical = null;
		var req = new Request({
			url: self.options.cgipath,
			method: "post",
			data: {
				"method": "eval",
				"name": document.getElement("li.tab.active").get("text").slice(0, -1),
				"kscript": text
			},
			onRequest: function() {
				var getreq = new Request({
					url: self.options.cgipath,
					method: "get",
					data: {
						"method": "getUID"
					},
					onSuccess: function(retUID) {
						periodical = new Request({
							url: self.options.cgipath.slice(0, -9) + "data/tmp/" + retUID + ".out",
							method: "get",
							initialDelay: 1000,
							delay: 1000,
							limit: 8000,
							onComplete: function(responseText) {
								//console.log("complete");
								self.options.result.set("html", "");
								var inputtxt = new Element("span", {
									"class": "stdout",
									html: self.escapeText(responseText)
								});
								inputtxt.inject(self.options.result);
							}
						});
						periodical.startTimer();
					}
				});
				getreq.send();
			},
			onSuccess: function(data) {
				periodical.stopTimer();
				self.options.result.set("html", "");
				var json = JSON.decode(data)["item"];
				for (var i = 0; i < json.length; i++) {
					var key = json[i].key;
					var val = json[i].value;
					if (val.length > 0) {
						//console.log("add result " + key + val);
						var inputtxt = new Element("span", {
							"class": key,
							html: self.escapeText(val)
						});
						inputtxt.inject(self.options.result);
						var br = new Element("br");
						br.inject(self.options.result);
					}
				}
				setTimeout(function(){self.options.eval.setProperty("disabled", false)}, 1000);
			}
		});
		req.send();
	}
});

window.addEvent("domready", function() {
	var aspen = new Aspen({
		url: "http://localhost/aspen/",
		cgipath: "http://localhost/aspen/cgi-bin/aspen.cgi",
		tabpane: document.id("tabpane"),
		eval: document.id("eval"),
		new: document.id("new"),
		open: document.id("open"),
		signout: document.id("sign_out"),
		result: document.id("result")
	});
	window.fireEvent("resize", 0);
});

window.addEvent("resize", function(index) {
	var height = window.getSize().y;
	if (height >= 275) {
		if (index == undefined) {
			var editors = document.getElements(".CodeMirror");
			for (var i = 0; i < editors.length; i++) {
				editors[i].setStyle("height", (height - 236) + "px");
			}
		} else {
			document.getElements(".CodeMirror")[index].setStyle("height", (height - 236) + "px");
		}
	}
});
