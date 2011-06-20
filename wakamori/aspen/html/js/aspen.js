/*
  aspen.js: Aspen js library
  
  started by utrhira
  modifed by chen_ji, shinpei_NKT
*/



var Aspen;
if (!Aspen) Aspen = {};

(function() {
/*
configuration : this is temporary.
*/
	var cgiDir = "../cgi-bin";

	var requestflag;
	var myCodeMirror;
	var logouted;
	var keyevent = function(editor, key) {
		//// Hook into ctrl-space
		//if (key.keyCode == 32 && (key.ctrlKey || key.metaKey) && !key.altKey) {
		//	key.stop();
		//	return AutoCompletion.startComplete(myCodeMirror);
		//}
		// Hook into Shift-Enter
		if (key.type == "keydown") {
			if (key.keyCode == 13) {
				if (key.shiftKey) {
					if (Aspen.requestAllowed()) {
						var text = editor.getValue();
						if (text.length > 0) {
							Aspen.postScript(text);
						}
					}
					key.returnValue = false;
				} else {
					Aspen.allowRequest();
				}
			} else if (!key.shiftKey) {
				Aspen.allowRequest();
			}
		}
	};
	Aspen.isLogouted = function() {
		return logouted;
	};
	Aspen.setLogout = function(flag) {
		logouted = flag;
	}
	Aspen.defineActions = function() {
		$("#eval")[0].onclick = function() {
			if (Aspen.requestAllowed()) {
				var text = myCodeMirror.getValue();
				if (text.length > 0) {
					Aspen.postScript(text);
				}
			}
		};
		/*
		$("#save")[0].onclick = function() {
			Aspen.saveCookie();
			$.ajax({
				type: "POST",
				url: "../cgi-bin/aspen.cgi?method=save",
				data: {
					"kscript": myCodeMirror.getValue()
				},
				success: function(data) {
					var uid = $.cookie("UID");
					var sid = $.cookie("SID");
					var filename = "../cgi-bin/scripts/" + uid + "/us_" + sid + ".k";
					document.location.href = filename;
				}
			});
		};
		*/

/*		$("#file")[0].onchange = function() {
			var fileList = $("#file")[0].files;
			var reader = new FileReader();
			var fsize = fileList[0].size;
			if (fsize <= 1024 * 16) {
				reader.readAsText(fileList[0], "utf-8");
				reader.onload = function(e) {
					myCodeMirror.setValue(e.target.result);
				};
				Aspen.saveCookie();
				Aspen.allowRequest();
				$("#result").empty();
				$("<span/>").attr("class", "message").append("file opened.").appendTo("#result");
			} else {
				$("#result").empty();
				$("<span/>").attr("class", "stderr").append("file size is too big:").appendTo("#result");
				$("<span/>").attr("class", "stderr").append(fsize).appendTo("#result");
				$("<span/>").attr("class", "stderr").append(" bytes, we only accept 16KiB.").appendTo("#result");
			}
		};

*/
		$("#new")[0].onclick = function() {
			myCodeMirror.setValue("");
			Aspen.saveCookie();
		};
		
		$("#forward")[0].onclick = function() {
			$.ajax({
				type: "POST",
				url: ""+ cgiDir + "/aspen.cgi?method=forward",
				data: {
					"sid": $.cookie("SID"),
					"uid": $.cookie("UID")
				},
				success: function(data) {
					var uid = $.cookie("UID");
					var sid = data["sid"];
					//console.log("after ajax:" + sid);
					if (sid == "none") { 
						$("#result").empty();
						$("<span/>").attr("class", "message").append("there is no forwarding scripts.").appendTo("#result");
						return;
					}
					/* overwrite current cookie */ 
					var date = new Date();
					date.setTime(date.getTime() + ((30 - date.getTimezoneOffset()) * 60 * 1000)); // 30 minutes
					$.cookie("SID", sid, {
						expires: date,
						path: "/"
					});
					var url =  cgiDir + "/scripts/" + uid + "/us_" + sid + ".k";
					$.get(url, function(data) {
						myCodeMirror.setValue(data);
						/* temporary */
						Aspen.saveCookie();
					});
					Aspen.allowRequest();
				}
			});
		};

		$("#rewind")[0].onclick = function() {
			//console.log("before ajax:" + $.cookie("SID"))
			$.ajax({
				type: "POST",
				url: "" + cgiDir + "/aspen.cgi?method=rewind",
				data: {
					"sid": $.cookie("SID"),
					"uid": $.cookie("UID")
				},
				success: function(data) {
					var uid = $.cookie("UID");
					var sid = data["sid"];
					//console.log("after ajax:" + sid);
					if (sid == "none") {
						$("#result").empty();
						$("<span/>").attr("class", "message").append("there is no previous scripts.").appendTo("#result");
						return;
					}
					/* overwrite current sid */
					var date = new Date();
					date.setTime(date.getTime() + ((30 - date.getTimezoneOffset()) * 60 * 1000)); // 30 minutes
					$.cookie("SID", sid, {
						expires: date,
						path: "/"
					});
					var url = cgiDir + "/scripts/" + uid + "/us_" + sid + ".k";
					$.get(url, function(data) {
						myCodeMirror.setValue(data);
						/* temporary */
						Aspen.saveCookie();
					});
					Aspen.allowRequest();
				}
			});
		};

		$("#logout")[0].onclick = function() {
			Aspen.setLogout(true);
			Aspen.deleteCookie();
		};
	};

	Aspen.start = function() {
		myCodeMirror = CodeMirror.fromTextArea($("#code")[0], {
			lineNumbers: true,
			matchBrackets: true,
			mode: "text/konoha",
			onKeyEvent: keyevent
		});
		Aspen.loadCookie();
		Aspen.saveCookie();
		Aspen.allowRequest();
		Aspen.defineActions();
		Aspen.setLogout(false);
	};

	Aspen.allowRequest = function() {
		requestflag = true;
	};

	Aspen.denyRequest = function() {
		requestflag = false;
	};

	Aspen.requestAllowed = function() {
		return requestflag;
	};

	Aspen.getText = function() {
		return myCodeMirror.getValue();
	};

	Aspen.loadCookie = function() {
		if ($.cookie("CODE")) {
			myCodeMirror.setValue($.cookie("CODE"));
		}
	};

	Aspen.saveCookie = function() {
		var date = new Date();
		date.setTime(date.getTime() + ((30 - date.getTimezoneOffset()) * 60 * 1000)); // 30 minutes
		$.cookie("CODE", myCodeMirror.getValue(), {
			expires: date,
			path: "/"
		});
	};

	Aspen.deleteCookie = function() {
		$.cookie("CODE", "", {
			expires: -1,
			path: "/"
		});
	};

	Aspen.postScript = function(text) {
		$("#result").empty();
		$("<span/>").attr("class", "message").append("Evaluating...").appendTo("#result");
		Aspen.denyRequest();
		Aspen.saveCookie();
		$.PeriodicalUpdater(
			cgiDir + "/scripts/" + $.cookie("UID") + "/us_" + $.cookie("SID") + ".out",
			{
				method: "GET",
				frequency: 1
			},
			function(data) {
				//console.log(data);
				if (data.length > 0) {
					$("#result").empty();
					var inputtxt = $("<span/>").attr("class", "stdout");
					inputtxt = inputtxt.html(data.replace(/\r\n/g, "<br />").replace(/(\n|\r)/g, "<br />"));
					$("#result").append(inputtxt);
				}
			}
		);
		$.ajax({
			type: "POST",
			url: cgiDir + "/aspen.cgi?method=eval",
			data: {
				"kscript": text
			},
			success: function(data) {
				clearTimeout(PeriodicalTimer);
				$("#result").empty();
				//console.log(data);
				var json = data["item"]
				for (var i = 0; i < json.length; i++) {
					var key = json[i].key;
					var val = json[i].value;
					if (val.length > 0) {
						var inputtxt = $("<span/>").attr("class", key);
						inputtxt = inputtxt.html(val.replace(/\r\n/g, "<br />").replace(/(\n|\r)/g, "<br />"));
						$("#result").append(inputtxt);
						$("#result").append("<br />");
					}
				}
			}
		});
	};
})();

$(function() {
	Aspen.start();
	window.onbeforeunload = function() {
		if (!Aspen.isLogouted() && $.cookie("CODE") != Aspen.getText()) {
			return "Script is not saved. Exit anyway?";
		}
	};
	/*
	function addTab(link) {
		// hide other tabs
		$("#tabs li").removeClass("current");
		$("#content p").hide();
		// add new tab and related content
		$("#tabs").append("<li class=\"current\"><a class=\"tab\" id=\"" +
			$(link).attr("rel") + "\" href=\"#\">" + $(link).html() +
			"</a><a href=\"#\" class=\"remove\">x</a></li>");
		$("#content").append("<p id=\"" + $(link).attr("rel") +
			"_content\">" + $(link).attr("title") + "</p>");
		// set the newly added tab as current
		$("#" + $(link).attr("rel") + "_content").show();
	}
	$("#documents a").click(function() {
		addTab($(this));
	});
	$('#tabs a.tab').live('click', function() {
		// Get the tab name
		var contentname = $(this).attr("id") + "_content";
		// hide all other tabs
		$("#content p").hide();
		$("#tabs li").removeClass("current");
		// show current tab
		$("#" + contentname).show();
		$(this).parent().addClass("current");
	});
	$('#tabs a.remove').live('click', function() {
		// Get the tab name
		var tabid = $(this).parent().find(".tab").attr("id");
		// remove tab and related content
		var contentname = tabid + "_content";
		$("#" + contentname).remove();
		$(this).parent().remove();
	});
	// If tab already exist in the list, return
	if ($("#" + $(link).attr("rel")).length != 0)
		return;
	// if there is no current tab and if there are still tabs left, show the first one
	if ($("#tabs li.current").length == 0 && $("#tabs li").length > 0) {
		// find the first tab
		var firsttab = $("#tabs li:first-child");
		firsttab.addClass("current");
		// get its link name and show related content
		var firsttabid = $(firsttab).find("a.tab").attr("id");
		$("#" + firsttabid + "_content").show();
	} 
	*/
});
