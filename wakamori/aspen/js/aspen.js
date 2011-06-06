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
			var uid = $.cookie("UID");
			var sid = $.cookie("SID");
			var filename = "./cgi/scripts/" + uid + "/us_" + sid + ".k";
			document.location.href = filename;
		};
		$("#file")[0].onchange = function() {
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
		var date = new Date();
		date.setTime(date.getTime() + (30 * 60 * 1000)); // 30 minutes
		$.cookie("code", myCodeMirror.getValue(), {
			expires: date,
			path: "/"
		});
	};
	Aspen.postScript = function(text) {
		$("#result").empty();
		$("<span/>").attr("class", "message").append("Evaluating...").appendTo("#result");
		Aspen.denyRequest();
		Aspen.saveCookie();
		$.PeriodicalUpdater(
			"./cgi/scripts/" + $.cookie("UID") + "/us_" + $.cookie("SID") + ".out",
			{
				method: "GET",
				frequency: 1
			},
			function(data) {
				if (data.length > 0) {
					$("#result").empty();
					$("<span/>").attr("class", "stdout").append(data).appendTo("#result");
				}
			}
		);
		//$.PeriodicalUpdater(
		//	"./cgi/scripts/" + $.cookie("UID") + "/us_" + $.cookie("SID") + ".err",
		//	{
		//		method: "GET",
		//		frequency: 1
		//	},
		//	function(data) {
		//		$(".message").empty();
		//		$(".stderr").text(data);
		//		//$(".stderr").empty();
		//		//$("<span/>").attr("class", "stderr").append(data).appendTo("#result");
		//	}
		//);
		$.ajax({
			type: "POST",
			url: "./cgi/aspen.cgi?method=eval",
			data: {
				"kscript": text
			},
			success: function(data) {
				clearTimeout(PeriodicalTimer);
				$("#result").empty();
				var json = JSON.parse(data);
				for (var i = 0; i < json.length; i++) {
					var key = json[i].key;
					var val = json[i].value;
					if (val.length > 0) {
						$("<span/>").attr("class", json[i].key).append(json[i].value).appendTo("#result");
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
		if ($.cookie("code") != Aspen.getText()) {
			return "Script is not saved. Exit anyway?";
		}
	}
});
