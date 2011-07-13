using konoha.visual.*;

class Puzzle {
	Scene window;
	Application app;
	Camera c;
	int timer_id;
	Point prev_point;
	Rect[] panels;

	Puzzle() {
		app = new Application();
		window = new Scene();
		window.setItemIndexMethod(Scene.NoIndex);
		window.setSceneRect(0, 0, 600, 600);
		window.setBackgroundColor(new Color("black"));
		Timer t = new Timer(100, delegate(this, capture));
		c = new Camera(0);
		for (int i = 0; i < 9; i++) {
			Rect panel = new Rect(0, 0, 100, 100);
			panel.setMousePressEvent(delegate(this, mousePressEvent));
			panel.setMouseReleaseEvent(delegate(this, mouseReleaseEvent));
			panel.setMouseMoveEvent(delegate(this, mouseMoveEvent));
			panels.add(panel);
			window.addItem(panel);
		}
		int panel_num = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (panel_num > 9) break;
				panels[panel_num].translate(110 * i + 50, 110 * j + 50);
				panel_num++;
			}
		}
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
		Texture[] textures = t.split(3, 3);
		for (int i = 0; i < |textures|; i++) {
			textures[i].setRect(new Rect(0, 0, 100, 100));
			panels[i].setTexture(textures[i]);
		}
	}

	void mousePressEvent(Rect o, MouseEvent event) {
		Point p = event.lastScreenPos();
		prev_point = p;
	}

	void mouseMoveEvent(Rect o, MouseEvent event) {
		Point p = event.lastScreenPos();
		int dx = p.x - prev_point.x;
		int dy = p.y - prev_point.y;
		o.translate(dx, dy);
		prev_point = p;
	}

	void mouseReleaseEvent(Rect o, MouseEvent event) {
		//Point p = event.lastScreenPos();
	}
}

void main(String[] args)
{
	Puzzle p = new Puzzle();
	p.show();
}
