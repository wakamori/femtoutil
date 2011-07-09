using konoha.visual.*;

class Puzzle {
	Scene window;
	Application app;
	Camera c;
	int timer_id;
	
	Puzzle() {
		app = new Application();
		window = new Scene();
		window.setItemIndexMethod(Scene.NoIndex);
		window.setSceneRect(0, 0, 600, 600);
		window.setBackgroundColor(new Color("black"));
		//window.setMousePressEvent(delegate(this, mousePressEvent));
		//window.setMouseReleaseEvent(delegate(this, mouseReleaseEvent));
		//window.setMouseMoveEvent(delegate(this, mouseMoveEvent));
		Timer t = new Timer(1 / 60, delegate(this, capture));
		c = new Camera(0);
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
		Texture t = c.queryFrame();
		Texture[] panels = t.split(3, 3);
		int half_size = |panels| / 2 - 1;
		int panel_num = 0;
		for (int i = 0; i < half_size; i++) {
			for (int j = 0; j < half_size; j++) {
				if (panel_num > |panels|) break;
				panels[panel_num].setRect(new Rect(0, 0, 100, 100));
				panels[panel_num].translate(110 * i + 50, 110 * j + 50);
				window.addItem(panels[panel_num]);
				panel_num++;
			}
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
