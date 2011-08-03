#include <iostream>
#include <QGridLayout>
#include <QApplication>
#include <QWidget>
#include "Scribble.h"

int main(int argc, char** argv) 
{
	QApplication app(argc, argv);
	QWidget window;
	Scribble *s = new Scribble;

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(s, 0, 0);

	//QObject::connect(s, SIGNAL(clicked()), s, SLOT(mouseMoveEvent()));
	//QObject::connect(s, SIGNAL(clicked()), s, SLOT(mousePressEvent()));
	//QObject::connect(s, SIGNAL(clicked()), s, SLOT(mouseReleaseEvent()));

	window.setWindowTitle("Paint");
	window.setLayout(layout);
	window.show();
	return app.exec();
}
