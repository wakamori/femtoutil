using konoha.visual.*;

class DrawScene {
	Scene scene;
	Pen pen;
	int start_x;
	int start_y;
	int prev_x;
	int prev_y;
	boolean drawable;
	Array<Point> obj;
	World world;

	DrawScene () {
		this.scene = new Scene();
		this.pen = createPen();
		this.scene.setSceneRect(0, 0, 600, 600);
		this.scene.setMousePressEvent(delegate(this, mousePressEvent));
		this.scene.setMouseReleaseEvent(delegate(this, mouseReleaseEvent));
		this.scene.setMouseMoveEvent(delegate(this, mouseMoveEvent));
		this.drawable = false;
		this.world = new World();
		this.obj = [];
	}

	void drawLineTo(int x, int y) {
		line = new Line(prev_x, prev_y, x, y);
		line.setPen(this.pen);
		this.scene.addItem(line);
		Point p = obj[obj.size-1];
		//print "Obj (" + p.x + "," + p.y + ")";
		prev_x = x;
		prev_y = y;
	}

	void mousePressEvent(Scene scene, MouseEvent event) {
		print scene.class;
		Point p = event.scenePos();
		start_x = p.x;
		start_y = p.y;
		obj.add(p);
		prev_x = p.x;
		prev_y = p.y;
		this.drawable = true;
	}
	
	void mouseMoveEvent(Scene scene, MouseEvent event) {
		if (drawable) {
			Point p = event.scenePos();
			obj.add(p);
			drawLineTo(p.x, p.y);
		}
	}
	void mouseReleaseEvent(Scene scene, MouseEvent event) {
		Point p = event.scenePos();
		drawLineTo(p.x, p.y);
		drawLineTo(start_x, start_y);
		this.drawable = false

		obj.add(p);
		//for(int i = 0; i < obj.size; i++) {
		//	OUT <<< "(" <<< obj[i].x <<< "," <<< obj[i].y <<< ")" <<< EOL;
		//}
		o = new ComplexItem(obj);
		obj = [];
		o.setDensity(1);
		this.world.add(o);
		this.scene.addComplexItem(o);
	}

	Pen createPen() {
		pen = new Pen();
		s = "black";
		pen.setColor(new Color(s));
		pen.setWidth(10);
		pen.setStyle(Pen.SolidLine);
		pen.setCapStyle(Pen.FlatCap);
		return pen;
	}
}

void main(String[] args)
{
	app = new Application();
	dscene = new DrawScene();

	view = new View(dscene.scene);
	view.show();
	dscene.world.start();
	app.exec();
}
