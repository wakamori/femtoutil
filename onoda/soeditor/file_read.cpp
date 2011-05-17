#include "file_read.h"

FileRead::FileRead()
{
	QLabel *fileLabel = new QLabel(tr("File:"));

	edit = new QLineEdit;
	edit->setMaxLength(LINE_SIZE);
	connect(edit, SIGNAL(returnPressed()), this, SLOT(lineLoad()));

	save_button = new QPushButton(tr("&Save"));
	load_button = new QPushButton(tr("&Load"));
	connect(save_button, SIGNAL(clicked()), this, SLOT(buttonSave()));
	connect(load_button, SIGNAL(clicked()), this, SLOT(buttonLoad()));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(fileLabel);
	layout->addWidget(edit);
	layout->addWidget(save_button);
	layout->addWidget(load_button);
	setLayout(layout);
}
  
void FileRead::lineLoad()
{
	QString fileName = edit->text();
	QFile file(fileName);

	if (fileName.isEmpty()) {
		return;
	} else {
		if (!file.open(QIODevice::ReadOnly)) {
			return;
		}
		QTextStream in(&file);
		linebuffer = in.readAll();
		std::cout << qPrintable(linebuffer) << std::endl;
	}
}

void FileRead::buttonSave()
{
	QString fileName = QFileDialog::getSaveFileName(this,
													tr("Save to File"), "",
													tr("all (*)"));
	QFile file(fileName);

	if (fileName.isEmpty()) {
		return;
	} else {
		if (!file.open(QIODevice::WriteOnly)) {
			return;
		}
		QTextStream out(&file);
		out << linebuffer;
		std::cout << qPrintable(linebuffer) << std::endl;
	}
}

void FileRead::buttonLoad()
{
	QString fileName = QFileDialog::getOpenFileName(this,
													tr("Load from File"), "",
													tr("all (*)"));
	QFile file(fileName);

	if (fileName.isEmpty()) {
		return;
	} else {
		if (!file.open(QIODevice::ReadOnly)) {
			return;
		}
		QTextStream in(&file);
		linebuffer = in.readAll();
		std::cout << qPrintable(linebuffer) << std::endl;
	}
}
