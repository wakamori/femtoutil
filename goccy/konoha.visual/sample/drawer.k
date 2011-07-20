using konoha.visual.*;

class DrawWindow {
	Application app;
	Scene window;
	Pen pen;
	int start_x;
	int start_y;
	int prev_x;
	int prev_y;
	boolean drawable;
	Array<Point> obj;
	Array<Line> lines;
	World world;

	DrawWindow () {
		app = new Application();
		window = new Scene();
		pen = createPen();
		window.setSceneRect(0, 0, 600, 600);
		window.setMousePressEvent(delegate(this, mousePressEvent));
		window.setMouseReleaseEvent(delegate(this, mouseReleaseEvent));
		window.setMouseMoveEvent(delegate(this, mouseMoveEvent));
		drawable = false;
		world = new World();
		Rect r = new Rect(0, 500, 600, 10);
		world.add(r);
		window.addItem(r);
		obj = [];
		lines = [];
	}

	void drawLineTo(int x, int y) {
		Line line = new Line(prev_x, prev_y, x, y);
		line.setPen(pen);
		window.addItem(line);
		lines.add(line);
		Point p = obj[obj.size-1];
		//print "Obj (" + p.x + "," + p.y + ")";
		prev_x = x;
		prev_y = y;
	}

	void mousePressEvent(Scene window, MouseEvent event) {
		Point p = event.scenePos();
		start_x = p.x;
		start_y = p.y;
		obj.add(p);
		prev_x = p.x;
		prev_y = p.y;
		drawable = true;
	}
	
	void mouseMoveEvent(Scene window, MouseEvent event) {
		if (drawable) {
			Point p = event.scenePos();
			obj.add(p);
			drawLineTo(p.x, p.y);
		}
	}
	
	Color getRandomColor() {
		String clr = "#";
		for (int i = 0; i < 3; i++) {
			clr += "%x"(100 + Int.random(155));
		}
		return new Color(clr);
	}
	
	void mouseReleaseEvent(Scene w, MouseEvent event) {
		Point p = event.scenePos();
		drawLineTo(p.x, p.y);
		drawLineTo(start_x, start_y);
		this.drawable = false
		obj.add(p);
		//for(int i = 0; i < obj.size; i++) {
		//	OUT <<< "(" <<< obj[i].x <<< "," <<< obj[i].y <<< ")" <<< EOL;
		//}
		print |obj|;
		if (|obj| > 2) {
			ComplexItem c = new ComplexItem(obj);
			c.setColor(getRandomColor());
			c.setDensity(1);
			world.add(c);
			window.addComplexItem(c);
		}
		obj = [];
		foreach (Line line in lines) {
			window.removeItem(line);
		}
		lines = [];
	}

	Pen createPen() {
		pen = new Pen();
		s = "black";
		pen.setColor(new Color(s));
		pen.setWidth(10);
		pen.setStyle(Pen.SolidLine);
		pen.setCapStyle(Pen.RoundCap);
		return pen;
	}
	
	void show() {
		View view = new View(window);
		view.setRenderHint(Painter.Antialiasing);
		view.show();
		world.start();
		app.exec();
	}
}

void main(String[] args)
{
	window = new DrawWindow();
	window.show();
}
