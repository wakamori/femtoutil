/* modified by wakamori */

CodeMirror.defineMode("konoha", function(config, parserConfig) {
	var indentUnit = config.indentUnit, cpp = parserConfig.useCPP;
	var multiLineStrings = parserConfig.multiLineStrings;
	var $vars = parserConfig.$vars, atAnnotations = parserConfig.atAnnotations;

	var keywords = function() {
		function kw(type) {return {type: type, style: "k-keyword"};}
		var A = kw("keyword a"), B = kw("keyword b"), C = kw("keyword c");
		var D = kw("keyword d"), E = kw("keyword e"), O = kw("operator"), T = kw("type");
		var M = {type: "atom", style: "k-atom"};
		return {
			"if": A, "while": A, "for": A, "foreach": A,
			"do": B, "else": B, "finally": B, "try": B,
			"assert": C, "break": C, "continue": C, "new": C, "return": C, "throw": C, "print": C,
			"const": D, "final": D, "private": D, "public": D, "register": D, "static": D,
			"import": E, "include": E, "using": E,
			"and": O, "exists": O, "instanceof": O, "not": O, "or": O, "typeof": O,
			"boolean": T, "byte": T, "char": T, "double": T, "dyn": T, "int": T, "long": T, "float": T,
			"short": T, "var": T,
			"false": M, "null": M, "true": M, "this": M,
			"assure": kw("assure"),
			"case": kw("case"),
			"catch": kw("catch"),
			"class": kw("class"),
			"default": kw("default"),
			"extends": kw("extends"),
			"format": kw("format"),
			"function": kw("function"),
			"goto": kw("goto"),
			"namespace": kw("namespace"),
			"script": kw("script"),
			"switch": kw("switch"),
			"this": kw("this")
		};
	}();

	var isOperatorChar = /[+\-*&%=<>!?|]/;
	function chain(stream, state, f) {
		state.tokenize = f;
		return f(stream, state);
	}

	var type;
	function ret(tp, style) {
		type = tp;
		return style;
	}

	function tokenBase(stream, state) {
		var ch = stream.next();
		if (ch == '"' || ch == "'")
			return chain(stream, state, tokenString(ch));
		else if (/[\[\]{}\(\),;\:\.]/.test(ch))
			return ret(ch);
		else if (ch == "#" && cpp && state.startOfLine) {
			stream.skipToEnd();
			return ret("directive", "k-preprocessor");
		}
		else if (/\d/.test(ch)) {
			stream.eatWhile(/[\w\.]/)
				return ret("number", "k-number");
		}
		else if (ch == "/") {
			if (stream.eat("*")) {
				return chain(stream, state, tokenComment);
			}
			else if (stream.eat("/")) {
				stream.skipToEnd();
				return ret("comment", "k-comment");
			}
			else {
				stream.eatWhile(isOperatorChar);
				return ret("operator");
			}
		}
		else if (isOperatorChar.test(ch)) {
			stream.eatWhile(isOperatorChar);
			return ret("operator");
		}
		else if (atAnnotations && ch == "@") {
			stream.eatWhile(/[\w\$_]/);
			return ret("annotation", "k-annotation");
		}
		else if ($vars && ch == "$") {
			stream.eatWhile(/[\w\$_]/);
			return ret("word", "k-var");
		}
		else {
			stream.eatWhile(/[\w\$_]/);
			if (keywords && keywords.propertyIsEnumerable(stream.current())) {
				var word = keywords[stream.current()];
				return ret(word["type"], word["style"]);
			}
			return ret("word", "k-word");
		}
	}

	function tokenString(quote) {
		return function(stream, state) {
			var escaped = false, next, end = false;
			while ((next = stream.next()) != null) {
				if (next == quote && !escaped) {end = true; break;}
				escaped = !escaped && next == "\\";
			}
			if (end || !(escaped || multiLineStrings))
				state.tokenize = tokenBase;
			return ret("string", "k-string");
		};
	}

	function tokenComment(stream, state) {
		var maybeEnd = false, ch;
		while (ch = stream.next()) {
			if (ch == "/" && maybeEnd) {
				state.tokenize = tokenBase;
				break;
			}
			maybeEnd = (ch == "*");
		}
		return ret("comment", "k-comment");
	}
	
	function Context(indented, column, type, align, prev) {
		this.indented = indented;
		this.column = column;
		this.type = type;
		this.align = align;
		this.prev = prev;
	}
	
	function pushContext(state, col, type) {
		return state.context = new Context(state.indented, col, type, null, state.context);
	}
	function popContext(state) {
		return state.context = state.context.prev;
	}

	// Interface

	return {
		startState: function(basecolumn) {
						return {
							tokenize: tokenBase,
							context: new Context((basecolumn || 0) - indentUnit, 0, "top", false),
							indented: 0,
							startOfLine: true
						};
					},
		token: function(stream, state) {
					var ctx = state.context;
					if (stream.sol()) {
						if (ctx.align == null) ctx.align = false;
						state.indented = stream.indentation();
						state.startOfLine = true;
					}
					if (stream.eatSpace()) return null;
					var style = state.tokenize(stream, state);
					if (type == "comment") return style;
					if (ctx.align == null) ctx.align = true;
					if ((type == ";" || type == ":") && ctx.type == "statement") popContext(state);
					else if (type == "{") pushContext(state, stream.column(), "}");
					else if (type == "[") pushContext(state, stream.column(), "]");
					else if (type == "(") pushContext(state, stream.column(), ")");
					else if (type == "}") {
						if (ctx.type == "statement") ctx = popContext(state);
						if (ctx.type == "}") ctx = popContext(state);
						if (ctx.type == "statement") ctx = popContext(state);
					}
					else if (type == ctx.type) popContext(state);
					else if (ctx.type == "}" || ctx.type == "top") pushContext(state, stream.column(), "statement");
					state.startOfLine = false;
					return style;
			},
		indent: function(state, textAfter) {
					if (state.tokenize != tokenBase) return 0;
					var firstChar = textAfter && textAfter.charAt(0), ctx = state.context, closing = firstChar == ctx.type;
					if (ctx.type == "statement") return ctx.indented + (firstChar == "{" ? 0 : indentUnit);
					else if (ctx.align) return ctx.column + (closing ? 0 : 1);
					else return ctx.indented + (closing ? 0 : indentUnit);
				},
		electricChars: "{}"
	};
});

CodeMirror.defineMIME("text/konoha", {name: "konoha", atAnnotations: true});
