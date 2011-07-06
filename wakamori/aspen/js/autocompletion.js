var AutoCompletion;
if (!AutoCompletion) AutoCompletion = {};

(function () {
	// Minimal event-handling wrapper.
	function stopEvent() {
		if (this.preventDefault) {this.preventDefault(); this.stopPropagation();}
		else {this.returnValue = false; this.cancelBubble = true;}
	}
	function addStop(event) {
		if (!event.stop) event.stop = stopEvent;
		return event;
	}
	function connect(node, type, handler) {
		function wrapHandler(event) {handler(addStop(event || window.event));}
		if (typeof node.addEventListener == "function") {
			node.addEventListener(type, wrapHandler, false);
		} else {
			node.attachEvent("on" + type, wrapHandler);
		}
	}

	function forEach(arr, f) {
		for (var i = 0, e = arr.length; i < e; ++i) {
			f(arr[i]);
		}
	}

	AutoCompletion.startComplete = function(editor) {
		// We want a single cursor position.
		if (editor.somethingSelected()) {
			return;
		}
		// Find the token at the cursor
		var cur = editor.getCursor(false);
		var token = editor.getTokenAt(cur);
		var tprop = token;
		// If it's not a 'word-style' token, ignore the token.
		if (!/^[\w$_]*$/.test(token.string)) {
			token = tprop = {
				start: cur.ch,
				end: cur.ch,
				string: "",
				state: token.state,
				className: token.string == "." ? "js-property" : null
			};
		}
		// If it is a property, find out what it is a property of.
		while (tprop.className == "js-property") {
			tprop = editor.getTokenAt({
				line: cur.line,
				ch: tprop.start
			});
			if (tprop.string != ".") {
				return;
			}
			tprop = editor.getTokenAt({
				line: cur.line,
				  ch: tprop.start
			});
			if (!context) {
				var context = [];
			}
			context.push(tprop);
		}
		var completions = getCompletions(token, context);
		if (!completions.length) {
			return;
		}
		function insert(str) {
			editor.replaceRange(str, {line: cur.line, ch: token.start}, {line: cur.line, ch: token.end});
		}
		// When there is only one completion, use it directly.
		if (completions.length == 1) {
			insert(completions[0]);
			return true;
		}

		// Build the select widget
		var complete = document.createElement("div");
		complete.className = "completions";
		var sel = complete.appendChild(document.createElement("select"));
		sel.multiple = true;
		for (var i = 0; i < completions.length; ++i) {
			var opt = sel.appendChild(document.createElement("option"));
			opt.appendChild(document.createTextNode(completions[i]));
		}
		sel.firstChild.selected = true;
		sel.size = Math.min(10, completions.length);
		var pos = editor.cursorCoords();
		complete.style.left = pos.x + "px";
		complete.style.top = pos.yBot + "px";
		document.body.appendChild(complete);
		// Hack to hide the scrollbar.
		if (completions.length <= 10)
			complete.style.width = (sel.clientWidth - 1) + "px";

		var done = false;
		function close() {
			if (done) {
				return;
			}
			done = true;
			complete.parentNode.removeChild(complete);
		}
		function pick() {
			insert(sel.options[sel.selectedIndex].value);
			close();
			setTimeout(function(){editor.focus();}, 50);
		}
		connect(sel, "blur", close);
		connect(sel, "keydown", function(event) {
			var code = event.keyCode;
			// Enter and space
			if (code == 13 || code == 32) {
				event.stop();
				pick();
			}
			// Escape
			else if (code == 27) {
				event.stop();
				close();
				editor.focus();
			}
			else if (code != 38 && code != 40) {
				close();
				editor.focus();
				setTimeout(AutoCompletion.startComplete, 50);
			}
		});
		connect(sel, "dblclick", pick);

		sel.focus();
		// Opera sometimes ignores focusing a freshly created node
		if (window.opera) {
			setTimeout(function(){if (!done) sel.focus();}, 100);
		}
		return true;
	};

	var stringProps = ("concat convert encode endsWith equals get getSize indexOf isASCII lastIndexOf " +
			"match replace search split startsWith substring trim").split(" ");
	var arrayProps = ("add clear get getSize indexOf insert lastIndexOf pop remove reverse send set " +
			"setAll shuffle swap").split(" ");
	/* var funcProps = ("prototype apply call bind").split(" "); */
	var keywords = ("if while for foreach do else finally try assert break continue new return throw " +
			"print const final private public register static import include using and exists " +
			"instanceof not or typeof boolean byte char double dyn int long float short var false " +
			"null true this assure case catch class default extends format function goto namespace " +
			"script switch this").split(" ");

	function getCompletions(token, context) {
		var found = [], start = token.string;
		function maybeAdd(str) {
			if (str.indexOf(start) == 0) found.push(str);
		}
		function gatherCompletions(obj) {
			if (typeof obj == "string") forEach(stringProps, maybeAdd);
			else if (obj instanceof Array) forEach(arrayProps, maybeAdd);
			/* else if (obj instanceof Function) forEach(funcProps, maybeAdd); */
			for (var name in obj) maybeAdd(name);
		}

		if (context) {
			// If this is a property, see if it belongs to some object we can
			// find in the current environment.
			var obj = context.pop(), base;
			if (obj.className == "js-variable")
				base = window[obj.string];
			else if (obj.className == "js-string")
				base = "";
			else if (obj.className == "js-atom")
				base = 1;
			while (base != null && context.length)
				base = base[context.pop().string];
			if (base != null) gatherCompletions(base);
		}
		else {
			// If not, just look in the window object and any local scope
			// (reading into JS mode internals to get at the local variables)
			for (var v = token.state.localVars; v; v = v.next) maybeAdd(v.name);
			gatherCompletions(window);
			forEach(keywords, maybeAdd);
		}
		return found;
	}
})();
