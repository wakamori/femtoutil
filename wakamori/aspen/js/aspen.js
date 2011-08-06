/*
  aspen.js: Aspen js library

 version:
  0.0.5 : reverted shadowbox (chen_ji)
  started by utrhira
  modifed by chen_ji, shinpei_NKT
*/

var Aspen = new Class({
	initialize: function(opt) {
		var self = this;
		var url = opt["url"];
		var codeElem = opt["code"];
		if (cgipath) {
			var cgipath = opt["cgipath"];
		} else {
			var cgipath = url + "cgi-bin/";
		}
		var evalElem = opt["eval"];
		var newElem = opt["new"];
		var openElem = opt["open"];
		var signoutElem = opt["signout"];
		var resultElem = opt["result"];

		var keyEvent = function(editor, key) {
			if (key.type == "keydown" && key.keyCode == 13 && key.shiftKey) {
				if (evalElem.getProperty("disabled") == false) {
					//var mySlide = new Fx.Slide("result");
					//mySlide.slideIn();
					var text = editor.getValue();
					if (text.length > 0) {
						self.postScript(evalElem, resultElem, cgipath, text);
					}
				}
				key.returnValue = false;
			}
		};
		var editor = CodeMirror.fromTextArea(codeElem, {
			lineNumbers: true,
			matchBrackets: true,
			mode: "text/konoha",
			onKeyEvent: keyEvent
		});

		// load script from aspen db
		var loadReq = new Request({
			url: cgipath + "aspen.cgi",
			method: "get",
			data: {
				"method": "load",
				"time": new Date().getTime() // for IE
			},
			onSuccess: function(responseText) {
				if (responseText == "") {
					editor.setValue("print \"hello, Konoha\";");
				} else {
					editor.setValue(responseText);
				}
			}
		});
		loadReq.send();

		// set actions
		evalElem.addEvent("click", function() {
			if (evalElem.getProperty("disabled") == false) {
				//mySlide.slideIn();
				var text = editor.getValue();
				//console.log(text);
				if (text.length > 0) {
					self.postScript(evalElem, resultElem, cgipath, text);
				}
			}
		});
		newElem.addEvent("click", function() {
			var req = new Request({
				url: cgipath + "aspen.cgi",
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
		openElem.addEvent("click", function() {
			var req = new Request({
				url: cgipath + "aspen.cgi",
				method: "get",
				data: {
					"method": "open",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					Shadowbox.init({
						onFinish: function() {
							var dirs = document.getElements("a.dir");
							for (var i = 0; i < dirs.length; i++) {
								dirs[i].addEvent("click", function() {
									this.getParent().getElement("ul").toggle();
								});
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
		signoutElem.addEvent("click", function() {
			var req = new Request({
				url: cgipath + "aspen.cgi",
				method: "get",
				data: {
					"method": "logout",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(data) {
					document.location = url;
				}
			});
			req.send();
		});
	},

	/*
	// set dom actions
	setActions: function() {
		var self = this;
		var changeScript = function(responseText) {
			//console.log(Cookie.read("UID"));
			//console.log(Cookie.read("SID"));
			//console.log(responseText);
			if (responseText == "latest") {
				document.id("result").set("html", "");
				var msg = new Element("span", {
					"class": "message",
					html: "there is no forwarding scripts."
				});
				msg.inject("result");
			} else if (responseText == "oldest") {
			} else {
				self.cm.setValue(responseText);
			}
		};
		*/
		/*
		document.id("forward").addEvent("click", function() {
			var req = new Request({
				url: self.cgi_dir + "/aspen.cgi",
				method: "get",
				data: {
					"method": "forward",
					"time": new Date().getTime() // for IE
				},
				onSuccess: changeScript
			});
			req.send();
		});
	},
	*/

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
					html: '<a href="#" class="dir">' + childdirs[j] + "/</a>"
				}));
			}
			for (var j = 0; j < childfiles.length; j++) {
				dir.getElement("ul").grab(new Element("li", {
					html: '<a href="#" class="file">' + childfiles[j] + "</a>"
				}));
			}
		}
		// console.log(ret.getElementById("filelist"));
		var tmp = new Element("div");
		tmp.grab(ret.getElementById("filelist"));
		return tmp.get("html");
	},

	// post script and eval
	postScript: function(evalElem, resultElem, cgipath, text) {
		var self = this;
		resultElem.set("html", "");
		var inputtxt = new Element("span", {
			"class": "message",
			html: "Running ..."
		});
		inputtxt.inject(resultElem);
		evalElem.setProperty("disabled", true);
		var periodical;
		var req = new Request({
			url: cgipath + "aspen.cgi",
			method: "post",
			data: {
				"method": "eval",
				"kscript": text
			},
			onRequest: function() {
				periodical = new Request({
					url: cgipath + "scripts/" + Cookie.read("UID") + "/us_" + Cookie.read("SID") + ".out",
					method: "get",
					initialDelay: 1000,
					delay: 1000,
					limit: 8000,
					onComplete: function(responseText) {
						//console.log("complete");
						resultElem.set("html", "");
						var inputtxt = new Element("span", {
							"class": "stdout",
							html: self.escapeText(responseText)
						});
						inputtxt.inject(resultElem);
					}
				});
				periodical.startTimer();
			},
			onSuccess: function(data) {
				periodical.stopTimer();
				resultElem.set("html", "");
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
						inputtxt.inject(resultElem);
						var br = new Element("br");
						br.inject(resultElem);
					}
				}
				setTimeout(function(){evalElem.setProperty("disabled", false)}, 1000);
			}
		});
		req.send();
	}
});

window.addEvent("domready", function() {
	var aspen = new Aspen({
		url: "http://localhost/aspen/",
		cgipath: "http://localhost/aspen/cgi-bin/",
		code: document.id("code"),
		eval: document.id("eval"),
		new: document.id("new"),
		open: document.id("open"),
		signout: document.id("sign_out"),
		result: document.id("result")
	});
});
