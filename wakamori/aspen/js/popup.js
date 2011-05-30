// edited by uh;

var Popup;
if (!Popup) Popup = {};

(function() {
    var pp = Popup;
    pp.selected;
    pp.offsetX;
    pp.offsetY;

    function onMouseDown(e) {
        if (!this.id) { return true; }
        pp.selected = document.getElementById(this.id);
        var offX = parseInt(pp.selected.style.left);
        var offY = parseInt(pp.selected.style.top);
        if (isNaN(offX)) offX = 0;
        if (isNaN(offY)) offY = 0;
        pp.offsetX = e.pageX - offX;
        pp.offsetY = e.pageY - offY;
        return false;
    }

    function onMouseMove(e) {
        if (!pp.selected) { return true; }
        pp.selected.style.left = (e.pageX - pp.offsetX)+"px";
        pp.selected.style.top  = (e.pageY - pp.offsetY)+"px";
        return false;
    }

    function onMouseUp(e) {
        pp.selected = null;
        pp.offsetX = 0;
        pp.offsetY = 0;
    }

    function onDblClick(e) {
        pp.remove(this.id);
        onMouseUp(e);
    }

    pp.create = function(id, inner, w, h) {
        var elem = document.createElement("div");
        elem.setAttribute("id", id);
        elem.setAttribute("class", "popup");
        if (inner) {
            var type = typeOf(inner);
            if (type == "element") {
                elem.appendChild(inner);
            } else if (type == "string") {
                ielem = document.createElement("div");
                ielem.innerText = inner;
                elem.appendChild(ielem);
            }
        }
        elem.onmousedown = onMouseDown;
        elem.ondblclick = onDblClick;
        document.onmousemove = onMouseMove;
        document.onmouseup = onMouseUp;
        document.body.appendChild(elem);
        var width = (w) ? parseInt(w) : elem.offsetWidth;
        var height = (h) ? parseInt(h) : elem.offsetHeight;
        if (w) elem.style.width = width;
        if (h) elem.style.height = height;
        elem.style.left = ((window.innerWidth - width)/2)+"px";
        elem.style.top = ((window.innerHeight - height)/2)+"px";
    };

    pp.remove = function(id) {
        var elem = document.getElementById(id);
        if (elem) {
            if (elem.getAttribute("class") == "popup") {
                document.body.removeChild(elem);
            }
        }
    };

}());
