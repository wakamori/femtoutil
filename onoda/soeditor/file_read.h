#ifndef FILE_READ_H
#define FILE_READ_H

#include <QtGui>
#include <qstring.h>
#include <string>
#include <iostream>
#include <QtDebug>
#include <fstream>
#include <QTextStream>

#define LINE_SIZE 64

extern QString linebuffer;

class FileRead : public QWidget
{
	Q_OBJECT

public:
	FileRead();
private slots:
	void lineLoad();
	void buttonLoad();
	void buttonSave();
private:
	QLineEdit *edit;
	QPushButton *save_button;
	QPushButton *load_button;
};


#endif //FILE_READ_H
