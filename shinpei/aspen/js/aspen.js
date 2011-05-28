// edited by uh;

var Aspen;
if (!Aspen) Aspen = {};

(function() {
    var requestflag = true;
    var resnum = 0;
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
})();

CodeMirror.defaults.onKeyEvent = function (editor, key) {
    if (key.type == "keypress" && key.shiftKey && key.keyIdentifier == "Enter") {
        if (Aspen.requestAllowed()) {
            var text = editor.getValue();
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (xhr.readyState == XMLHttpRequest.DONE) {
                    if (xhr.status == 200) {
                        var res = document.createElement("div");
                        res.addClass("codemirror");
                        res.innerText = xhr.responseText;
                        Popup.create("result"+ Aspen.resnum(), res);
                    }
                }
            };
            xhr.open("POST", "./index.html", true); // async
            xhr.setRequestHeader("Content-Type", "text/plain");
            xhr.send("kscript=" + text);
            setTimeout("Aspen.allowRequest()", 3000);
            console.log(xhr);
        }
    }
};
