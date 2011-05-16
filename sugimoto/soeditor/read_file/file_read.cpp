#include "file_read.h"

FileRead::FileRead()
{
	QLabel *fileLabel = new QLabel(tr("File:"));

	edit = new QLineEdit;
	edit->setMaxLength(LINE_SIZE);
	connect(edit, SIGNAL(returnPressed()), this, SLOT(lineLoad()));

	save_button = new QPushButton("&Save");
	save_button->setToolTip(tr("Save to file"));
	load_button = new QPushButton("&Load");
	load_button->setToolTip(tr("Load file"));
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
	if (!fileName.isEmpty()) {
		return;
	} else {
		if(!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
	}
	QDataStream in(&file);
	in >> g_linebuffer;
}


void FileRead::buttonSave()
{
	QString fileName = QFileDialog::getSaveFileName(this,
													tr("Save File"), "",
													tr("all (*)"));
	QFile file(fileName);
	if (fileName.isEmpty()) {
		return;
	} else {
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),file.errorString());
			return;
		}
	}
	QDataStream out(&file);
	out << g_linebuffer;
}


void FileRead::buttonLoad()
{
	QString fileName = QFileDialog::getOpenFileName(this,
													tr("Load File"), "",
													tr("all (*)"));
	QFile file(fileName);
	if (!fileName.isEmpty()) {
		return;
	} else {
		if(!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
	}
	QDataStream in(&file);
	in >> g_linebuffer;
}
