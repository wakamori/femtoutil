// edited by utrhira
// modified by chen_ji

var Aspen;
if (!Aspen) Aspen = {};

(function() {
	var requestflag;
	var myCodeMirror;
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
		$("#eval")[0].onclick = function() {
			if (Aspen.requestAllowed()) {
				var text = myCodeMirror.getValue();
				if (text.length > 0) {
					Aspen.postScript(text);
				}
			}
		};
		$("#save")[0].onclick = function() {
			//if (Aspen.requestAllowed()) {
			//	var text = myCodeMirror.getValue();
			//	if (text.length > 0) {
			//		Aspen.postScript(text);
			//	}
			//}
		};
		$("#file")[0].onchange = function() {
			var fileList = $("#file")[0].files;
			var reader = new FileReader();
			reader.readAsText(fileList[0], "utf-8");
			reader.onload = function(e) {
				myCodeMirror.setValue(e.target.result);
			};
			Aspen.saveCookie();
		};
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
		if ($.cookie("code")) {
			myCodeMirror.setValue($.cookie("code"));
		}
	};
	Aspen.saveCookie = function() {
		$.cookie("code", null);
		$.cookie("name", null);
		var date = new Date();
		date.setTime(date.getTime() + (30 * 60 * 1000)); // 30 minutes
		$.cookie("code", myCodeMirror.getValue(), {
			expires: date,
			path: "/"
		});
		$.cookie("name", date.getTime());
	};
	Aspen.postScript = function(text) {
		$("#result").text("Evaluating...");
		$.ajax({
			type: "POST",
			url: "./cgi/aspen.cgi",
			data: {
				"kscript": text
			},
			success: function(data) {
				$("#result").empty()
				var json = JSON.parse(data);
				for (var i = 0; i < json.length; i++) {
					var key = json[i].key;
					var val = json[i].value;
					if (val.length > 0) {
						$("<span/>").attr("class", key).append(val).appendTo("#result");
						$("#result").append("<br />");
					}
				}
			}
		});
		$.ajax({
			type: "POST",
			url: "./cgi/aspen.cgi",
			data: {
				"kscript": text,
			},
			success: function(data) {
				clearTimeout(PeriodicalTimer);
				var json = JSON.parse(data);
				$("<span/>").attr("class", json[0].key).append(json[0].value).appendTo("#result");
				$("#result").append("<br />");
			}
		});
	};
})();

$(function() {
	Aspen.start();
	window.onbeforeunload = function() {
		if ($.cookie("code") != Aspen.getText()) {
			return "Script is not saved. Exit anyway?";
		}
	}
	$("#sign_up").lightbox_me({
		centered: true,
		closeClick: false,
		closeEsc: false,
		onLoad: function() {
			$("#sign_up").find("input:first").focus();
		}
	});
	$("#cancel").click( function() {
			document.location = "http://www.ubicg.ynu.ac.jp/lab"
		}
	);
});


