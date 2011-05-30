// edited by utrhira

var Aspen;
if (!Aspen) Aspen = {};

function escapeAll(text) {
    return escape(text).replace(new RegExp( "\\+", "g" ),"%2B");
}

(function() {
    var requestflag = true;
    var resnum = 0;
    Aspen.timeout = 3000; // 3sec. default
    Aspen.allowRequest = function() {
        requestflag = true;
    };
    Aspen.requestAllowed = function() {
        if (requestflag) {
            requestflag = false;
            return true;
        }
        return false;
    };
    Aspen.resnum = function() {
        return resnum++;
    };
    Aspen.postScript = function(text) {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
            if (xhr.readyState == XMLHttpRequest.DONE) {
                if (xhr.status == 200) {
                    var view = document.getElementById("result");
                    view.innerText = xhr.responseText;
                }
            }
        };
        xhr.open("POST", location.href, true); // async
        xhr.setRequestHeader("Content-Type", "text/plain");
        xhr.send("kscript=" + escapeAll(text));
        setTimeout("Aspen.allowRequest()", Aspen.timeout);
    };
})();

CodeMirror.defaults.onKeyEvent = function (editor, key) {
    if (key.type == "keydown" && key.shiftKey && key.keyIdentifier == "Enter") {
        if (Aspen.requestAllowed()) {
            var text = editor.getValue();
            if (text.length > 0) {
                Aspen.postScript(text);
                key.preventDefault();
            }
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
