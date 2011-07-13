/*
  aspen.js: Aspen js library
  
 version:
  0.0.3 : added shadowbox (chen_ji)
  started by utrhira
  modifed by chen_ji, shinpei_NKT
*/

window.onload = function() {
	Shadowbox.init();
	if (Cookie.read("UID") == null || Cookie.read("SID") == null) {
		Shadowbox.open({
			content: '<form id="login" action="./cgi-bin/aspen.cgi?method=login" method="post">'
			+ '<div class="info">Aspenを使用するには、Twitterアカウントでの認証が必要です。</div>'
			+ '<input id="sign_in_button" type="image" src="./img/sign-in-with-twitter-d.png" alt="Twitterで認証する"></form>',
			player: "html",
			height: 100
		});
		document.id("sb-nav").hide();
	}
};

var Aspen = new Class({
	cm: null,
	cgi_dir: null,
	loadReq: null,
	newReq: null,

	initialize: function() {
		var self = this;
		this.cgi_dir = "./cgi-bin";
		var keyEvent = function(editor, key) {
			if (key.type == "keydown" && key.keyCode == 13 && key.shiftKey) {
				if (document.id("eval").getProperty("disabled") == false) {
					//var mySlide = new Fx.Slide("result");
					//mySlide.slideIn();
					var text = editor.getValue();
					if (text.length > 0) {
						self.postScript(text);
					}
				}
				key.returnValue = false;
			}
		};
		this.cm = CodeMirror.fromTextArea(document.id("code"), {
			lineNumbers: true,
			matchBrackets: true,
			mode: "text/konoha",
			onKeyEvent: keyEvent
		});
		this.loadReq = new Request({
			url: self.cgi_dir + "/aspen.cgi",
			method: "get",
			data: {
				"method": "load",
				"time": new Date().getTime() // for IE
			},
			onSuccess: function(responseText) {
				if (responseText == "") {
					self.cm.setValue("print \"hello, Konoha\";");
				} else {
					self.cm.setValue(responseText);
				}
			}
		});
		this.newReq = new Request({
			url: self.cgi_dir + "/aspen.cgi",
			method: "get",
			data: {
				"method": "new",
				"time": new Date().getTime() // for IE
			},
			onSuccess: function() {
				self.cm.setValue("print \"hello, Konoha\";");
			}
		});
		this.loadScriptFromDB();
		this.setActions();
	},

	// load script from aspen db
	loadScriptFromDB: function() {
		this.loadReq.send();
	},

	// set dom actions
	setActions: function() {
		var self = this;
		//var mySlide = new Fx.Slide("result");
		//mySlide.hide();
		document.id("eval").addEvent("click", function() {
			//mySlide.slideIn();
			var text = self.cm.getValue();
			//console.log(text);
			if (text.length > 0) {
				self.postScript(text);
			}
		});
		document.id("new").addEvent("click", function() {
			self.newReq.send();
		});
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
		document.id("sign_out").addEvent("click", function() {
			var req = new Request({
				url: self.cgi_dir + "/aspen.cgi",
				method: "get",
				data: {
					"method": "logout",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function() {
					location.reload();
				}
			});
			req.send();
		});
		document.id("rewind").addEvent("click", function() {
			var req = new Request({
				url: self.cgi_dir + "/aspen.cgi",
				method: "get",
				data: {
					"method": "rewind",
					"time": new Date().getTime() // for IE
				},
				onSuccess: changeScript
			});
			req.send();
		});
		if (Cookie.read("UID") == null || Cookie.read("SID") == null) {
			// show login dialog
			document.id("titleheader").hide();
			document.id("info").hide();
			document.id("topmenu").hide();
			document.id("result").hide();
			document.id("file").hide();
			this.cm.toTextArea();
			document.id("code").hide();
			document.id("footer").hide();
		} else {
			// hide login dialog and show login status
			var req = new Request({
				url: self.cgi_dir + "/aspen.cgi",
				method: "get",
				data: {
					"method": "getInfo",
					"time": new Date().getTime() // for IE
				},
				onSuccess: function(responseText) {
					var json = JSON.decode(responseText);
					var img = new Element("img", {
						src: json["profile_image_url"],
						styles: {
							"vertical-align": "middle"
						}
					});
					document.id("user").set("html", json["name"] + " @" + json["screen_name"]);
					img.inject("user", "top");
				}
			});
			req.send();
		}
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

	// post script and eval
	postScript: function(text) {
		var self = this;
		document.id("result").set("html", "");
		var inputtxt = new Element("span", {
			"class": "message",
			html: "Running ..."
		});
		inputtxt.inject("result");
		document.id("eval").setProperty("disabled", true);
		var periodical;
		var req = new Request({
			url: self.cgi_dir + "/aspen.cgi",
			method: "post",
			data: {
				"method": "eval",
				"kscript": text
			},
			onRequest: function() {
				periodical = new Request({
					url: self.cgi_dir + "/scripts/" + Cookie.read("UID") + "/us_" + Cookie.read("SID") + ".out",
					method: "get",
					initialDelay: 1000,
					delay: 1000,
					limit: 8000,
					onComplete: function(responseText) {
						//console.log("complete");
						document.id("result").set("html", "");
						var inputtxt = new Element("span", {
							"class": "stdout",
							html: self.escapeText(responseText)
						});
						inputtxt.inject("result");
					}
				});
				periodical.startTimer();
			},
			onSuccess: function(data) {
				periodical.stopTimer();
				document.id("result").set("html", "");
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
						inputtxt.inject("result");
						var br = new Element("br");
						br.inject("result");
					}
				}
				setTimeout(function(){document.id("eval").setProperty("disabled", false)}, 1000);
			}
		});
		req.send();
	}
});

window.addEvent("domready", function() {
	var aspen = new Aspen();
});
