$(function() {
	$("#username.sprited").focus(function() {
		if(this.value == "your name") {
			$(this).val("").css("color", "black");
		}
	});
	$("#username.sprited").blur(function() {
		if(this.value == "") {
			$(this).val("your name").css("color", "gray");
		}
	});
	$("#loginform").lightbox_me({
		centered: true,
		closeClick: false,
		closeEsc: false,
		onLoad: function() {
			$("#username.sprited").focus();
		}
	});
});
