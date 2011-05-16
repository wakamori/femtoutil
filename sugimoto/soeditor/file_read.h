#ifndef FILE_READ_H
#define FILE_READ_H

#include <QtGui>
#include <qstring.h>
#include <string>
#include <iostream>
#include <QtDebug>
#include <fstream>

#define LINE_SIZE 64

extern QString g_linebuffer;

class FileRead : public QWidget
{
	Q_OBJECT

public:
	FileRead();
private slots:
	void lineLoad();
	void buttonSave();
	void buttonLoad();

private:
	QLineEdit *edit;
	QPushButton *load_button;
	QPushButton *save_button;
};


#endif //FILE_READ_H
