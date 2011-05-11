#include "soeditor.h" 

MainWindow::MainWindow(QWidget * iParent, Qt::WindowFlags iFlags) : QMainWindow(iParent, iFlags) {
	QTextEdit *m_TextEdit = new QTextEdit("", this); // (3) セントラルウィジット
	setCentralWidget(m_TextEdit);
	setWindowTitle(tr("soeditor"));
	resize(350,250);
	show(); 
}

