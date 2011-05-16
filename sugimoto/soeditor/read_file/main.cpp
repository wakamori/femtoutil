#include "file_read.h"

QString g_linebuffer;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	g_linebuffer = "hogehoge22222";

	FileRead *sample = new FileRead;

	sample->show();

	return app.exec();
}
