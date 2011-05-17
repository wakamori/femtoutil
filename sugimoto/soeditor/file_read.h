#ifndef FILE_READ_H
#define FILE_READ_H

#include <QtGui>
#include <qstring.h>
#include <string>
#include <iostream>
#include <QtDebug>
#include <fstream>

#define LINE_SIZE 64

class FileRead : public QWidget
{
	Q_OBJECT

public:
	QString retInput();
	FileRead();

private slots:
	void getString();

private:
	QLineEdit *edit;
	QString *input;
};


#endif //FILE_READ_H
