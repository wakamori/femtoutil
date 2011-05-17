#include "Widget.h"
#include <QtGui>
	Widget::Widget(QWidget * iParent, Qt::WindowFlags iFlags)
: QWidget(iParent, iFlags)
{
	makeWidgets();
}

void Widget::makeWidgets()
{
	int countTab = 0;
	m_FileLabel = new QLabel(tr("File : "));

	m_FileLineEdit = new QLineEdit;

	m_FileOpenButton = new QPushButton("Add");

	m_Tab = new QTabWidget(this);
	m_Tab->setUsesScrollButtons(true);
	m_Tab->setMovable(true);
	m_Tab->setTabsClosable(true);
	m_Tab->setDocumentMode(true);

	m_h_StatusLayout = new QHBoxLayout();
	m_h_StatusLayout->addWidget(m_FileLabel);
	m_h_StatusLayout->addWidget(m_FileLineEdit);
	m_h_StatusLayout->addWidget(m_FileOpenButton);
	m_c_TextLayout = new QHBoxLayout();
	m_c_TextLayout->addWidget(m_Tab);

	m_v_WholeLayout = new QVBoxLayout;
	m_v_WholeLayout->addLayout(m_h_StatusLayout);
	m_v_WholeLayout->addLayout(m_c_TextLayout);

	setLayout(m_v_WholeLayout);

	newTab(countTab);
	newTab(countTab);
	newTab(countTab);
	createAddTabAction();
	connect(m_Tab, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteWidget(int)));
	setAcceptDrops(false);
}

//void Widget::newTextField(int countTab)
//{
//
//}
void Widget::newTab(int countTab)
{
	QString title = tr("new text field") + QString::number(++countTab);
	QTextEdit *text = new QTextEdit();
	text->setPlainText(tr("Hello World !!"));
	m_Tab->addTab(text, title);
	m_Tab->setCurrentIndex(m_Tab->count() - 1);
	countTab ++;
}

void Widget::createAddTabAction()
{
m_AddTabAction = new QAction(this);
m_AddTabAction->setShortcut(tr("Ctrl+J"));
m_AddTabAction->setStatusTip(tr(""));
connect(m_AddTabAction, SIGNAL(triggered()), this, SLOT(newTab()));
}
