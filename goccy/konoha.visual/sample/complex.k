using konoha.visual.*;

Application app = new ();
World world = new (600, 600);
Scene scene = new ();
scene.setSceneRect(0, 0, 600, 600);

Array<Point> a = new (0);
a <<< new Point(100, 130);
a <<< new Point(100, 50);
a <<< new Point(50, 100);
a <<< new Point(60, 200);
a <<< new Point(140, 200);
a <<< new Point(150, 100);
ComplexItem c = new ComplexItem(a);
//Rect c = new (10, 10, 30, 30);
world.add(c);
scene.addComplexItem(c);

View view = new(scene);
view.setRenderHint(Painter.Antialiasing);
view.show();
world.start();
app.exec();
