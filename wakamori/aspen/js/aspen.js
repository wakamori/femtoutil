// edited by utrhira

var Aspen;
if (!Aspen) Aspen = {};

function escapeAll(text) {
	return escape(text).replace(new RegExp( "\\+", "g" ),"%2B");
}

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
		var xhr = new XMLHttpRequest();
		xhr.onreadystatechange = function() {
			if (xhr.readyState == 4) {
				if (xhr.status == 200) {
					var rtexts = xhr.responseText.split("\\\n");
					document.getElementById("stdout").innerText = rtexts[0];
					document.getElementById("stderr").innerText = rtexts[1];
					document.getElementById("message").innerText = rtexts[2];
				}
			}
		};
		xhr.open("POST", location.href, true); // async
		xhr.setRequestHeader("Content-Type", "text/plain");
		xhr.send("kscript=" + escapeAll(text));
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
