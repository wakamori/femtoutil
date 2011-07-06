using konoha.visual.*;

class PhysicsWorld {
	World world;
	Scene scene;
	
	PhysicsWorld() {
		world = new World(600, 600);
		scene = new Scene();
		scene.setSceneRect(0, 0, 600, 600);
	}
	
	void addStaticObject() {
		Color static_block_color = new Color("#90ee90");
		Rect ground = new Rect(200, 400, 200, 40);
		ground.setColor(static_block_color);
		Rect left_block = new Rect(200, 350, 10, 50);
		left_block.setColor(static_block_color);
		Rect right_block = new Rect(400 - 10, 350, 10, 50);
		right_block.setColor(static_block_color);
		Ellipse e = new Ellipse();
		e.setRect(new Rect(300, 300, 20, 20));
		e.setColor(new QColor("#999900"));
		ground.setRestitution(0);
		scene.addItem(ground);
		scene.addItem(left_block);
		scene.addItem(right_block);
		scene.addItem(e);
		world.add(ground);
		world.add(left_block);
		world.add(right_block);
		world.add(e);
	}

	void addDynamicObject() {
		for (int i = 0; i < 10; i++) {
			float f = 100.0;
			float r = f.random() * 100;
			Rect rect = new Rect(350 - r, 300 - r, 10, 10);
			rect.setDensity(1);
			//rect.setFriction(0.3);
			rect.setRestitution(0.1);
			rect.setColor(new Color("#87ceeb"));//155 - r, 200 - r, 255 - r));
			world.add(rect);
			scene.addItem(rect);
		}
		
		Ellipse e = new Ellipse();
		e.setRect(new Rect(250, 300, 20, 20));
		e.setColor(new Color("#eeee00"));
		e.setDensity(1.0);
		Text text = new Text("Hello World");
		text.setPosition(300, 100);
		text.setDensity(1.0);
		Texture texture = new Texture("sample/qt.jpg");
		texture.setRect(new Rect(200, 200, 50, 50));
		texture.setDensity(1);
		world.add(e);
		scene.addItem(e);
		world.add(text);
		scene.addItem(text);
		world.add(texture);
		scene.addItem(texture);
	}

	void show() {
		View view = new View(scene);
		view.setRenderHint(Painter.Antialiasing);
		view.show();
		world.start();
	}
}

void main(String[] args)
{
	Application app = new Application();
	PhysicsWorld world = new PhysicsWorld();
	world.addStaticObject();
	world.addDynamicObject();
	world.show();
	app.exec();
}
