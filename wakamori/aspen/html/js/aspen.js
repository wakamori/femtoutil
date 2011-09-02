/*
  aspen.js: Aspen js library

 version:
  0.0.6 : add tabpane
  started by utrhira
  modifed by chen_ji, shinpei_NKT
*/

var resizeEditor = function(index) {
	var height = window.getSize().y;
	if (height >= 275) {
		var editors = document.getElements(".CodeMirror");
		if (index == undefined) {
			editors.each(function(editor) {
				editor.setStyle("height", (height - 236) + "px");
			});
		} else if (editors.length > index) {
			editors[index].setStyle("height", (height - 236) + "px");
		}
	}
};

var Aspen = new Class({
	Implements: Options,

	options: {
		url: "http://127.0.0.1/aspen/",
		cgipath: "http://127.0.0.1/aspen/cgi-bin/aspen.k",
		tabpane: null,
		runbtn: null,
		"newbtn": null,
		openbtn: null,
		savebtn: null,
		signoutbtn: null,
		result: null,
		codemirror: new Array()
	},

	isEventAdded: false,
	isEvalCompleted: true,
	periodical: null,

	initialize: function(options) {
		var self = this;
		this.setOptions(options);
		//console.log(self.options);

		self.addTab("noname.k", 'print "hello, konoha";');

		var myTabPane = new TabPane(self.options.tabpane.get("id"), {
			contentSelector: "div.CodeMirror"
		});

		//// load script from aspen db
		//var loadReq = new Request({
		//	url: self.options.cgipath,
		//	method: "get",
		//	data: {
		//		"type": "load",
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
		self.options.runbtn.addEvent("click", function() {
			if (this.getProperty("disabled") == false) {
				//mySlide.slideIn();
				var editor = self.options.codemirror[self.getActiveTabIndex()];
				//console.log(editor);
				var text = editor.getValue();
				//console.log(text);
				if (text.length > 0) {
					self.options.runbtn.setProperty("disabled", true);
					self.options.runbtn.addClass("evaluating");
					self.postScript(text);
				}
			}
		});
		self.options.newbtn.addEvent("click", function() {
			self.addTab("noname.k", "");
			var index = self.options.tabpane.getElements("li.tab").length - 1;
			myTabPane.showTab(index);
			resizeEditor(index);
		});
		self.options.tabpane.addEvent("click:relay(.remove)", function(e) {
			e.stop();
			if (self.options.codemirror.length <= 1) {
				// don't remove tab but clear it
				self.options.codemirror[0].setValue("");
				this.getParent().set("html", 'noname.k<span class="remove">&times</span>');
			} else {
				var parent = this.getParent();
				var index = self.options.tabpane.getElements("li.tab").indexOf(parent);
				var textarea = self.options.tabpane.getElements(".dummy")[index];
				textarea.destroy();
				self.options.codemirror.splice(index, 1);
				myTabPane.closeTab(index);
			}
		});
		self.options.tabpane.addEvent("click:relay(.tab)", function(e) {
			//console.log("relaytab");
			//console.log("resize");
			var index = self.getActiveTabIndex();
			resizeEditor(index);
			//console.log("refresh");
			self.options.codemirror[index].refresh();
		});
		self.options.tabpane.addEvent("dblclick:relay(.tab)", function() {
			// name file
			var tab = this;
			var orig = tab.get("text").slice(0, -1);
			tab.set("html", "");
			var input = new Element("input", {
				"class": "box tabedit",
				"value": orig
			});
			input.addEvent("keydown", function(e) {
				//console.log("keydown");
				//console.log(e.key);
				if (e.key == "enter") {
					//console.log("blur");
					this.fireEvent("blur");
				}
			});
			input.addEvent("blur", function() {
				//console.log("blurred!");
				var val = input.get("value").trim();
				if (!val.match(/^[a-zA-Z0-9\.\-_]*$/)) {
					alert("You cannot use '" + val + "' as filename.");
				} else {
					this.destroy();
					if (val != "") {
						tab.set("html", val + '<span class="remove">&times</span>');
					} else {
						tab.set("html", orig + '<span class="remove">&times</span>');
					}
				}
			});
			input.inject(tab).select();
		});
		self.options.openbtn.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"type": "open",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					Shadowbox.init({
						onFinish: function() {
							self.setActions(myTabPane);
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
		self.options.signoutbtn.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"type": "signout",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					document.location = self.options.url;
				}
			});
			req.send();
		});
		self.options.savebtn.addEvent("click", function() {
			var req = new Request({
				url: self.options.cgipath,
				method: "post",
				data: {
					"type": "save",
					"name": document.getElement("li.tab.active").get("text").slice(0, -1),
					"kscript": self.options.codemirror[self.getActiveTabIndex()].getValue()
				},
				onSuccess: function(data) {
					//console.log(data);
					self.options.result.set("html", "");
					var inputtxt = new Element("span", {
						"class": "message",
						html: data
					}).inject(self.options.result);
				}
			});
			req.send();
		});
	},

	// get active editor from codemirror array
	getActiveTabIndex: function() {
		var tabs = this.options.tabpane.getElements("li.tab");
		//console.log(tabs);
		var activeTab = this.options.tabpane.getElement("li.tab.active");
		//console.log(activeTab);
		//console.log(tabs.indexOf(activeTab));
		return tabs.indexOf(activeTab);
	},

	// add new tab
	addTab: function(tabname, content) {
		var self = this;
		if (self.options.codemirror.length > 10) {
			// too many tabs, addTab request rejected
			return;
		}
		var keyEvent = function(editor, key) {
			if (key.type == "keydown" && key.keyCode == 13 && key.shiftKey) {
				if (self.options.runbtn.getProperty("disabled") == false) {
					//var mySlide = new Fx.Slide("result");
					//mySlide.slideIn();
					var text = editor.getValue();
					if (text.length > 0) {
						self.options.runbtn.setProperty("disabled", true);
						self.options.runbtn.addClass("evaluating");
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
		//console.log(tabpane);
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
		tabpane.getElement("ul").grab(tabli);
		myeditor.setValue(content);
		self.options.codemirror.push(myeditor);

		// codemirror.getWrapperElement() doesn't work on IE
		// myeditor.getWrapperElement().setStyle("display", "none");
		var editordiv = document.getElements(".CodeMirror")[self.options.codemirror.length - 1];
		editordiv.setStyle("display", "none");
		//tabpane.showTab(tabpane.getElements("li").length - 1);

		//console.log(self.options.codemirror);
	},

	// set filemanager action
	setActions: function(myTabPane) {
		var self = this;
		document.getElements("div.dir").each(function(el) {
			el.addEvent("click", function() {
				this.getParent().getElement("ul").toggle();
			});
		});
		var files = document.getElements("div.file");
		var openfunc = function(el) {
			if (el.getProperty("disabled") == true) return;
			el.setProperty("disabled", true);
			var filename = document.id("sb-nav-filename").get("value");
			var req = new Request({
				url: self.options.cgipath,
				method: "get",
				data: {
					"type": "load",
					"file": filename,
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(text) {
					self.addTab(filename, text);
					var index = self.options.tabpane.getElements("li.tab").length - 1;
					myTabPane.showTab(index);
					resizeEditor(index);
				}
			});
			req.send();
			Shadowbox.close();
			el.setProperty("disabled", false);
		};
		files.each(function (file) {
			file.addEvent("click", function() {
				var parents = this.getParents("li");
				if (parents.length <= 1) {
					document.id("sb-nav-filename").set("value", this.get("text"));
				} else {
					document.id("sb-nav-filename").set("value", this.getParents("li")[1].get("id").substring(5) + "/" + this.get("text"));
				}
				for (var i = 0; i < files.length; i++) {
					if (files[i].get("selected") == true) {
						files[i].set("selected", false);
						files[i].setStyle("background-color", "transparent");
						break;
					}
				}
				this.set("selected", true);
				this.setStyle("background-color", "yellow");
				//console.log(files.get("selected"));
			});
			file.addEvent("dblclick", function() {
				openfunc(this);
			});
		});
		if (self.isEventAdded) return;

		// following code is executed once
		self.isEventAdded = true;
		document.id("sb-nav-open").addEvent("click", function() {
			openfunc(this);
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
	getULStringfromArray: function(array) {
		// console.log(data);
		var ret = new Element("ul", {
			html: '<li id="dir_/">/</li>'
		});
		array.each(function (data) {
			if (data[0] == "/") {
				var dirname = "/";
			} else {
				var dirname = data[0] + "/"; // /dir1/
			}
			var childdirs = data[1]; // dir1, dir2
			var childfiles = data[2]; // xxx.k, ...
			var dir = ret.getElementById("dir_" + data[0]); // dir_/dir1
			if (data[0] == "/") {
				dir.set("html", dir.get("html") + '<ul id="filelist" class="filelist"></ul>');
			} else {
				dir.set("html", dir.get("html") + '<ul class="filelist" style="display: none;"></ul>');
			}
			// console.log(dir);
			childdirs.each(function (childdir) {
				dir.getElement("ul").grab(new Element("li", {
					id: "dir_" + dirname + childdir,
					html: '<div class="dir">' + childdir + "/</div>"
				}));
			});
			childfiles.each(function (childfile) {
				dir.getElement("ul").grab(new Element("li", {
					html: '<div class="file">' + childfile + "</div>"
				}));
			});
		});
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
		var req = new Request({
			url: self.options.cgipath,
			method: "post",
			data: {
				"type": "eval",
				"name": document.getElement("li.tab.active").get("text").slice(0, -1),
				"kscript": text
			},
			onRequest: function() {
				var getreq = new Request({
					url: self.options.cgipath,
					method: "get",
					data: {
						"type": "getUID"
					},
					onSuccess: function(retUID) {
						//self.isEvalCompleted = false;
						if (self.periodical != null) {
							return;
						}
						self.periodical = new Request({
							url: self.options.cgipath.slice(0, -7) + "data/tmp/" + retUID + ".out",
							method: "get",
							initialDelay: 1000,
							delay: 1000,
							limit: 8000,
							onComplete: function(responseText) {
								//console.log("complete");
								//console.log(self.isEvalCompleted);
								if (self.isEvalCompleted) {
									self.isEvalCompleted = false;
									// kill itself
									this.stopTimer();
									self.periodical = null;
									return;
								}
								self.options.result.set("html", "");
								var inputtxt = new Element("span", {
									"class": "stdout",
									html: self.escapeText(responseText)
								});
								inputtxt.inject(self.options.result);
							}
						});
						self.periodical.startTimer();
					}
				});
				getreq.send();
			},
			onSuccess: function(data) {
				if (self.periodical != null) {
					self.periodical.stopTimer();
					self.periodical = null;
				} else {
					self.isEvalCompleted = true;
				}
				self.options.result.set("html", "");
				var json = JSON.decode(data)["item"];
				json.each(function (e) {
					var key = e.key;
					var val = e.value;
					if (val.length > 0) {
						//console.log("add result " + key + val);
						var inputtxt = new Element("span", {
							"class": key,
							//html: self.escapeText(val)
							html: val
						});
						inputtxt.inject(self.options.result);
						var br = new Element("br");
						br.inject(self.options.result);
					}
				});
				setTimeout(function() {
					self.options.runbtn.setProperty("disabled", false);
					self.options.runbtn.removeClass("evaluating");
				}, 1000);
			}
		});
		req.send();
	}
});

window.addEvent("domready", function() {
	var aspen = new Aspen({
		tabpane: document.id("tabpane"),
		runbtn: document.id("eval"),
		"newbtn": document.id("new"),
		openbtn: document.id("open"),
		savebtn: document.id("save"),
		signoutbtn: document.id("sign_out"),
		result: document.id("result")
	});
	//console.log("resize");
	resizeEditor(0);
});

window.addEvent("resize", resizeEditor);
