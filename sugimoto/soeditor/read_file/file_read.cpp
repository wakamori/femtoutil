#include "file_read.h"

FileRead::FileRead()
{
	edit = new QLineEdit;
	edit->setMaxLength(LINE_SIZE);

	input = new QString;
	
	connect(edit, SIGNAL(returnPressed()), this, SLOT(getString()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(edit);
	setLayout(layout);
}
  
void FileRead::getString()
{
	*input = edit->text();
	std::cout << (qPrintable(*input)) << std::endl; /*for debug*/

	std::ifstream ReadFile(qPrintable(*input));

	if (ReadFile.fail()) {
		std::cout << "file not found" << std::endl;
	} else {
		std::string temp;
		while(getline(ReadFile, temp)) {
			std::cout << temp << std::endl;
		}
	}
}

QString FileRead::retInput()
{ 
	return *input;
}

