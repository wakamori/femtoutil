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
                    var res = document.createElement("div");
                    res.addClass("codemirror");
                    res.innerText = xhr.responseText;
                    var txt = document.createElement("span");
                    txt.innerText = "Press Enter";
                    res.appendChild(txt);
                    Popup.create("result"+ Aspen.resnum(), res, window.outerWidth/3, window.outerHeight/3);
                }
            }
        };
        xhr.open("POST", location.href, true); // async
        xhr.setRequestHeader("Content-Type", "text/plain");
        xhr.send("kscript=" + escapeAll(text));
        setTimeout("Aspen.allowRequest()", Aspen.timeout);
    };
    Aspen.closeAllResults = function() {
        var pops = document.querySelectorAll(".popup");
        var len = pops.length;
        if (len > 0) {
            for (idx in pops) {
                var pop = pops[idx];
                var parent = pop.parentElement;
                if (parent) parent.removeChild(pop);
            }
        }
        return len;
    };
})();

CodeMirror.defaults.onKeyEvent = function (editor, key) {
    if (key.type == "keydown" && key.keyIdentifier == "Enter") {
        if (key.shiftKey) {
            if (Aspen.requestAllowed()) {
                var text = editor.getValue();
                if (text.length > 0) {
                    Aspen.postScript(text);
                    key.preventDefault();
                }
            }
        } else {
            if (Aspen.closeAllResults() > 0) {
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
