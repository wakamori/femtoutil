using konoha.visual.*;

class AR {
	Scene window;
	Application app;
	Camera c;
	int timer_id;
	Texture background;
	ComplexItem[] prev_list;
	World world;
	Texture prev_texture;

	AR() {
		app = new Application();
		window = new Scene();
		world = new World();
		window.setItemIndexMethod(Scene.NoIndex);
		window.setSceneRect(0, 0, 1200, 700);
		window.setBackgroundColor(new Color("black"));
		for (int i = 0; i < 100; i++) {
			int rand = Int.random(100);
			Rect rect = new Rect(350 - rand, 100 - rand, 50, 50);
			rect.setDensity(1);
			//rect.setFriction(0.3);
			rect.setRestitution(0.7);
			rect.setColor(new Color("#87ceeb"));//155 - r, 200 - r, 255 - r));
			rect.setZValue(4);
			world.add(rect);
			window.addItem(rect);
		}
		Timer t = new Timer(100, delegate(this, capture));
		c = new Camera(0);
		background = c.queryFrame();
		window.addItem(background);
		timer_id = t.start();
		prev_list = [];
	}

	void show() {
		View view = new View(window);
		view.setViewportUpdateMode(View.FullViewportUpdate);
		view.setRenderHint(Painter.Antialiasing);
		view.show();
		world.start();
		app.exec();
	}

	void capture(TimerEvent event) {
		if (prev_texture != null) {
			window.removeItem(prev_texture);
		}
		Texture t = c.queryFrame();
		Array<Array<Point>> objs = t.detectHuman(background);
		int objs_size = |objs|;
		for (int i = 0; i < |prev_list|; i++) {
			//window.removeComplexItem(prev_list[i]);
			world.remove(prev_list[i]);
		}
		prev_list = [];
		window.addItem(t);
		for (int i = 0; i < objs_size; i++) {
			Array<Point> obj = objs[i];
			int obj_size = |obj|;
			ComplexItem item = new ComplexItem(obj);
			//window.addComplexItem(item);
			world.add(item);
			prev_list.add(item);
		}
		prev_texture = t;
	}
}

void main(String[] args)
{
	AR ar = new AR();
	ar.show();
}
