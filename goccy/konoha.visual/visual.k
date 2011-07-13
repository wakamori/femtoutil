include nativelink;

@Native class Visual;
@Native class Point;
@Native class Item;
@Native class Group extends Item;
@Native class Ellipse extends Item;
@Native class Rect extends Item;
@Native class Texture extends Item;
@Native class Text extends Item;
@Native class ComplexItem extends Item;
@Native class TimeLine;
@Native class Transform;
@Native class Connector;
@Native class Color;
@Native class Brush;
@Native class Scene;
@Native class View;
@Native class Painter;
@Native class Application;
@Native class Timer;
@Native class TimerEvent;
@Native class World;
@Native class Shadow;
@Native class Camera;
@Native class MouseEvent;

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
void Rect.setShadow(Shadow s);
void Rect.setTexture(Texture t);
boolean Rect.isStatic();
void Rect.setMousePressEvent(Func<Rect,MouseEvent=>void> func);
void Rect.setMouseMoveEvent(Func<Rect,MouseEvent=>void> func);
void Rect.setMouseReleaseEvent(Func<Rect,MouseEvent=>void> func);

Texture Texture.new(String filepath);
void Texture.setRect(Rect r);
void Texture.setColor(Color c);
void Texture.setRotation(float rotation);
void Texture.setDensity(float density);
void Texture.setFriction(float friction);
void Texture.setRestitution(float restitution);
Texture[] Texture.split(int row, int col);
Array<Array<Point>> Texture.detectHuman(Texture background);
void Texture.setMousePressEvent(Func<Texture,MouseEvent=>void> func);
void Texture.setMouseMoveEvent(Func<Texture,MouseEvent=>void> func);
void Texture.setMouseReleaseEvent(Func<Texture,MouseEvent=>void> func);

Text Text.new(String text);
void Text.setPosition(int x, int y);
void Text.setColor(Color c);
void Text.setRotation(float rotation);
void Text.setDensity(float density);
void Text.setFriction(float friction);
void Text.setRestitution(float restitution);

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
void Scene.removeItem(Item i);
void Scene.addComplexItem(ComplexItem c);
void Scene.removeComplexItem(ComplexItem c);
//void Scene.setMousePressEvent();

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
void World.setBeginContactEvent(Func<dynamic,dynamic=>void> contact);
void World.setEndContactEvent(Func<dynamic,dynamic=>void> contact);

Shadow Shadow.new();

Camera Camera.new(int camera_num);
Texture Camera.queryFrame();

Point Point.new(int x, int y);
int Point.getX();
int Point.getY();

ComplexItem ComplexItem.new(Array<Point> obj);
void ComplexItem.setDensity(float f);
void ComplexItem.setPosition(int x, int y);

Point MouseEvent.lastScreenPos();
