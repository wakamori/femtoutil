// edited by utrhira
// modified by chen_ji

var Aspen;
if (!Aspen) Aspen = {};

(function() {
	var requestflag;
	var myCodeMirror;
	Aspen.start = function() {
		myCodeMirror = CodeMirror.fromTextArea($("#code")[0], {
			lineNumbers: true,
			matchBrackets: true,
			mode: "text/konoha",
			onKeyEvent: function(editor, key) {
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
			}
		});
		Aspen.loadCookie();
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
		};
		$("#file")[0].onchange = function() {
			var fileList = $("#file")[0].files;
			var reader = new FileReader();
			reader.readAsText(fileList[0], "utf-8");
			reader.onload = function(e) {
				myCodeMirror.setValue(e.target.result);
			};
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
	Aspen.loadCookie = function() {
		if ($.cookie("code")) {
			myCodeMirror.setValue($.cookie("code"));
		}
	};
	Aspen.saveCookie = function() {
		$.cookie("code", null);
		var date = new Date();
		date.setTime(date.getTime() + (30 * 60 * 1000)); // 30 minutes
		$.cookie("code", myCodeMirror.getValue(), {
			expires: date,
			path: "/"
		});
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
				$("#result").empty();
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
		Aspen.denyRequest();
		Aspen.saveCookie();
	};
})();

$(function() {
	Aspen.start();
});
