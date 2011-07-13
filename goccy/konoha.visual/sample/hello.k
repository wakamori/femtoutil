using konoha.visual.*;

void main(String[] args)
{
	app = new Application();
	scene = new Scene();
	scene.setSceneRect(0, 0, 600, 600);
	logo = new Texture("sample/ks_logo.png");
	logo.setRect(new Rect(0, 0, 100, 100));
	r = new Rect(100, 100, 100, 100);
	r.setTexture(logo);
	scene.addItem(r);
	view = new View(scene);
	view.show();
	app.exec();
}
