#include <QApplication>
#include "Window.h"

int main(int argc, char *argv[]) 
{
	QApplication application(argc, argv);
	MainWindow mainWindow;
	return application.exec();
}
