#include "Widget.h"
#include <QtGui>
	Widget::Widget(QWidget * iParent, Qt::WindowFlags iFlags)
: QWidget(iParent, iFlags)
{
	makeWidgets();
}

void Widget::makeWidgets()
{
	countTab = 0;
	m_FileLabel = new QLabel(tr("File : "));

	m_FileLineEdit = new QLineEdit;

	m_FileOpenButton = new QPushButton("Open");
	m_AddTabButton = new QPushButton("Add Tab");

	m_Tab = new QTabWidget(this);
	m_Tab->setUsesScrollButtons(true);
	m_Tab->setMovable(true);
	m_Tab->setTabsClosable(true);
	m_Tab->setDocumentMode(true);

	m_h_StatusLayout = new QHBoxLayout();
	m_h_StatusLayout->addWidget(m_FileLabel);
	m_h_StatusLayout->addWidget(m_FileLineEdit);
	m_h_StatusLayout->addWidget(m_FileOpenButton);
	m_h_StatusLayout->addWidget(m_AddTabButton);
	m_c_TextLayout = new QHBoxLayout();
	m_c_TextLayout->addWidget(m_Tab);

	m_v_WholeLayout = new QVBoxLayout;
	m_v_WholeLayout->addLayout(m_h_StatusLayout);
	m_v_WholeLayout->addLayout(m_c_TextLayout);

	setLayout(m_v_WholeLayout);


	connect(m_AddTabButton, SIGNAL(clicked()), this, SLOT(newTab(void)));
	connect(m_Tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	newTab();
	newTab();
	newTab();
	newTab();
	setAcceptDrops(false);
}

void Widget::newTab()
{
	QString title = tr("new text field") + QString::number(++countTab);
	QTextEdit *text = new QTextEdit();
	text->setPlainText(tr("Hello World !!"));
	m_Tab->addTab(text, title);
	m_Tab->setCurrentIndex(m_Tab->count() - 1);
	printf("debug\n");
}

void Widget::closeTab(int index)
{
m_Tab->removeTab(index);
}
