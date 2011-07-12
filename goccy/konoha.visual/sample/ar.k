using konoha.visual.*;

class Puzzle {
	Scene window;
	Application app;
	Camera c;
	int timer_id;
	Texture background;
	int count;
	Puzzle() {
		app = new Application();
		window = new Scene();
		window.setItemIndexMethod(Scene.NoIndex);
		window.setSceneRect(0, 0, 1200, 700);
		window.setBackgroundColor(new Color("black"));
		//window.setMousePressEvent(delegate(this, mousePressEvent));
		//window.setMouseReleaseEvent(delegate(this, mouseReleaseEvent));
		//window.setMouseMoveEvent(delegate(this, mouseMoveEvent));
		Timer t = new Timer(1 / 60.0, delegate(this, capture));
		c = new Camera(0);
		count = 0;
		background = c.queryFrame();
		window.addItem(background);
		timer_id = t.start();
	}

	void show() {
		View view = new View(window);
		view.setViewportUpdateMode(View.FullViewportUpdate);
		view.setRenderHint(Painter.Antialiasing);
		view.show();
		app.exec();
	}

	void capture(TimerEvent event) {
		count++;
		Texture t = c.queryFrame();
		Array<Array<Point>> objs = t.detectHuman(background);
		int objs_size = |objs|;
		//print objs_size;
		for (int i = 0; i < objs_size; i++) {
			Array<Point> obj = objs[i];
			int obj_size = |obj|;
			//print obj_size;
			ComplexItem item = new ComplexItem(obj);
			if (count == 10) {
				window.addComplexItem(item);
				//System.exit(0);
			}
			//for (int j = 0; j < obj_size; j++) {
			//Point p = obj[j];
				//print "(x, y) = " + "(" + p.x + ", " + p.y + ")";
			//}
		}
	}

	void mousePressEvent(dynamic o, MouseEvent event) {
		print "mousePressEvent";
		print o.class;
	}

	void mouseReleaseEvent(dynamic o, MouseEvent event) {
		print "mouseReleaseEvent";
		print o.class;
	}

	void mouseMoveEvent(dynamic o, MouseEvent event) {
		print "mouseMoveEvent";
		print o.class;
	}
}

void main(String[] args)
{
	Puzzle p = new Puzzle();
	p.show();
}
