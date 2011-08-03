using konoha.visual.*;

Application app = new ();
World world = new (500, 500);
Scene scene = new ();
scene.setSceneRect(0, 0, 500, 500);

Array<Point> a = new (0);
a <<< new Point(100, 0); a <<< new Point(0, 173); a <<< new Point(200, 173); a <<< new Point(100, 0);
a <<< new Point(100, 173); a <<< new Point(0, 200); a <<< new Point(0, 0); a <<< new Point(100, 173);
a <<< new Point(0, 0); a <<< new Point(100, 0); a <<< new Point(100, 100); a <<< new Point(0, 100);
a <<< new Point(60, 200); a <<< new Point(140, 150); a <<< new Point(150, 100);

ComplexItem c = new ComplexItem(a);
c.setColor(new Color("blue"));
c.setDensity(1);
world.add(c);
scene.addComplexItem(c);

Rect g = new Rect(0, 300, 500, 50);
g.setColor(new Color("lightblue"));
g.setRestitution(0.3);
world.add(g);
scene.addItem(g);

View view = new(scene);
view.setRenderHint(Painter.Antialiasing);
view.show();
world.start();
app.exec();
