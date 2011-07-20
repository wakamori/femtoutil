include lib:gluelink;

@Native class Visual;
@Native class Point;
@Native class Item;
@Native class Group extends Item;
@Native class Ellipse extends Item;
@Native class Rect extends Item;
@Native class Texture extends Item;
@Native class Text extends Item;
@Native class ComplexItem extends Item;
class Line extends Item;
@Native class TimeLine;
@Native class Transform;
@Native class Connector;
@Native class Pen;
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
@Native class Video;

TimeLine TimeLine.new(TimeLine _);
void TimeLine.setDuration(int duration);
void TimeLine.setCurveShape(int shape);
void TimeLine.valueChanged();
void TimeLine.start();

Transform Transform.new(Transform _);
void Transform.translate(int x, int y);
void Transform.rotate(int angle, int axis);

void Connector.connect(TimeLine from_func, int type, dynamic to_func);

Ellipse Ellipse.new(Ellipse _);
void Ellipse.setRect(Rect r);
void Ellipse.setColor(Color c);
void Ellipse.setZValue(int val);
int Ellipse.getZValue();
void Ellipse.setRotation(float rotation);
void Ellipse.setDensity(float density);
void Ellipse.setFriction(float friction);
void Ellipse.setRestitution(float restitution);

Rect Rect.new(int x, int y, int width, int height, Rect _);
void Rect.setColor(Color c);
void Rect.setRotation(float rotation);
void Rect.setDensity(float density);
void Rect.setFriction(float friction);
void Rect.setRestitution(float restitution);
void Rect.setShadow(Shadow s);
void Rect.setTexture(Texture t);
boolean Rect.isStatic();
void Rect.setZValue(int val);
void Rect.setMousePressEvent(Func<Rect,MouseEvent=>void> func);
void Rect.setMouseMoveEvent(Func<Rect,MouseEvent=>void> func);
void Rect.setMouseReleaseEvent(Func<Rect,MouseEvent=>void> func);

Texture Texture.new(String filepath, Texture _);
void Texture.setRect(Rect r);
void Texture.setColor(Color c);
void Texture.setRotation(float rotation);
void Texture.setDensity(float density);
void Texture.setFriction(float friction);
void Texture.setRestitution(float restitution);
Texture[] Texture.split(int row, int col);
Array<Array<Point>> Texture.detectHuman(Texture background, Point _);
void Texture.setMousePressEvent(Func<Texture,MouseEvent=>void> func);
void Texture.setMouseMoveEvent(Func<Texture,MouseEvent=>void> func);
void Texture.setMouseReleaseEvent(Func<Texture,MouseEvent=>void> func);

Text Text.new(String text, Text _);
void Text.setPosition(int x, int y);
void Text.setColor(Color c);
void Text.setRotation(float rotation);
void Text.setDensity(float density);
void Text.setFriction(float friction);
void Text.setRestitution(float restitution);

Line Line.new(int x1, int y1, int x2, int y2, Line _);
void Line.setColor(Color c);
void Line.setPen(Pen pen);
void Line.setRotation(float rotation);
void Line.setDensity(float density);
void Line.setFriction(float friction);
void Line.setRestitution(float restitution);
void Line.setShadow(Shadow s);
boolean Line.isStatic();

Color Color.new(String color, Color _);
Brush Brush.new(String color, Brush _);

Pen Pen.new(Pen _);
void Pen.setColor(Color color);
void Pen.setWidth(float width);
void Pen.setStyle(int style);
void Pen.setCapStyle(int style);
void Pen.setJoinStyle(int style);

void Item.translate(int x, int y);
void Item.setTransform(Transform t, boolean b);

Group Group.new(Group _);
void Group.addToGroup(Item i);

Scene Scene.new(Scene _);
void Scene.setItemIndexMethod(int idx);
void Scene.setBackgroundColor(Color color);
void Scene.setSceneRect(int x, int y, int width, int height);
void Scene.addItem(Item i);
void Scene.removeItem(Item i);
void Scene.addComplexItem(ComplexItem c);
void Scene.removeComplexItem(ComplexItem c);
void Scene.setMousePressEvent(Func<Scene,MouseEvent=>void> func);
void Scene.setMouseMoveEvent(Func<Scene,MouseEvent=>void> func);
void Scene.setMouseReleaseEvent(Func<Scene,MouseEvent=>void> func);

View View.new(Scene scene, View _);
void View.setViewportUpdateMode(int mode);
void View.setRenderHint(int hint);
void View.setBackgroundOpacity(int alpha);
void View.show();

Painter Painter.new(Painter _);

Application Application.new(String[] args, Application _);
void Application.exec();

Timer Timer.new(int interval, Func<TimerEvent=>void> func, Timer _);
int Timer.start();

World World.new(int width, int height, World _);
void World.add(Item i);
void World.start();
void World.setBeginContactEvent(Func<dynamic,dynamic=>void> contact);
void World.setEndContactEvent(Func<dynamic,dynamic=>void> contact);
void World.setGravity(float x, float y);
void World.remove(Item i);

Shadow Shadow.new(Shadow _);

Camera Camera.new(int camera_num, Camera _);
Texture Camera.queryFrame(Texture _);

Video Video.new(String filename, Camera _);
Texture Video.queryFrame(Texture _);

Point Point.new(int x, int y, Point _);
int Point.getX();
int Point.getY();

ComplexItem ComplexItem.new(Array<Point> obj, ComplexItem _);
void ComplexItem.setDensity(float f);
void ComplexItem.setPosition(int x, int y);
void ComplexItem.setColor(Color c);

Point MouseEvent.lastScreenPos(Point _);
Point MouseEvent.scenePos(Point _);
