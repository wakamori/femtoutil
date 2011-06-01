// edited by utrhira

var Aspen;
if (!Aspen) Aspen = {};

(function() {
	var requestflag = true;
	var resnum = 0;
	Aspen.allowRequest = function() {
		requestflag = true;
	};
	Aspen.denyRequest = function() {
		requestflag = false;
	}
	Aspen.requestAllowed = function() {
		return requestflag;
	};
	Aspen.resnum = function() {
		return resnum++;
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
				json = JSON.parse(data);
				for (var i = 0; i < json.length; i++) {
					var key = json[i].key;
					var val = json[i].value;
					$("<span/>").attr("class", key).append(val).appendTo("#result");
					$("#result").append("<br />");
				}
			}
		});
		Aspen.denyRequest();
	}
})();

CodeMirror.defaults.onKeyEvent = function(editor, key) {
	if (key.type == "keydown") {
		if (key.keyCode == 13) {
			if (key.shiftKey) {
				if (Aspen.requestAllowed()) {
					g_key = key;
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

window.onload = function() {
	var code = document.getElementById("code");
	var myCodeMirror = CodeMirror.fromTextArea(code, {
		lineNumbers: true,
		matchBrackets: true,
		mode: "text/konoha"
	});
	var evalbtn = document.getElementById("eval");
	evalbtn.onclick = function() {
		if (Aspen.requestAllowed()) {
			var text = myCodeMirror.getValue();
			if (text.length > 0) {
				Aspen.postScript(text);
			}
		}
	};
};
