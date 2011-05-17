#include <file_read.h>

QString linebuffer;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FileRead sample;
	sample.show();

	return a.exec();
}
