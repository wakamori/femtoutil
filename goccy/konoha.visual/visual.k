include nativelink;
@Native class Visual;
class Item;
class Group extends Item;
class Ellipse extends Item;
class Rect extends Item;
@Native class TimeLine;
class Transform;
class Connector;
class Color;
class Brush;
@Native class Scene;
@Native class View;
@Native class Painter;
class Application;
class Timer;
class TimerEvent;
class World;

TimeLine TimeLine.new();
void TimeLine.setDuration(int duration);
void TimeLine.setCurveShape(int shape);
void TimeLine.valueChanged();
void TimeLine.start();

Transform Transform.new();
void Transform.translate(int x, int y);
void Transform.rotate(int angle, int axis);

void Connector.connect(TimeLine from_func, int type, dynamic to_func);

Ellipse Ellipse.new();
void Ellipse.setRect(Rect r);
void Ellipse.setColor(Color c);
void Ellipse.setZValue(int val);
int Ellipse.getZValue();
void Ellipse.setRotation(float rotation);
void Ellipse.setDensity(float density);
void Ellipse.setFriction(float friction);
void Ellipse.setRestitution(float restitution);

Rect Rect.new(int x, int y, int width, int height);
void Rect.setColor(Color c);
void Rect.setRotation(float rotation);
void Rect.setDensity(float density);
void Rect.setFriction(float friction);
void Rect.setRestitution(float restitution);

Color Color.new(String color);
Brush Brush.new(String color);

void Item.translate(int x, int y);
void Item.setTransform(Transform t, boolean b);

Group Group.new();
void Group.addToGroup(Item i);

Scene Scene.new();
void Scene.setItemIndexMethod(int idx);
void Scene.setBackgroundColor(Color color);
void Scene.setSceneRect(int x, int y, int width, int height);
void Scene.addItem(Item i);

View View.new(Scene scene);
void View.setViewportUpdateMode(int mode);
void View.setRenderHint(int hint);
void View.show();

Painter Painter.new();

Application Application.new(String[] args);
void Application.exec();

Timer Timer.new(int interval, Func<TimerEvent=>void> func);
int Timer.start();

World World.new(int width, int height);
void World.add(Item i);
void World.start();
