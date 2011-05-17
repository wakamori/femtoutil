#include "Window.h"
#include "Widget.h"
#include <QtGui>

MainWindow::MainWindow(QWidget * iParent, Qt::WindowFlags iFlags) : QMainWindow(iParent, iFlags)
{
	m_Widget = new Widget(this);

	createActions();
	createMenus();
	createStatusBar();

	setCentralWidget(m_Widget);
	setWindowTitle(tr("Soeditor"));
	resize(1024,720);
	show(); 
}

void MainWindow::createActions()
{
	m_QuitAction = new QAction(this);
	m_QuitAction->setShortcut(tr("Ctrl+Q"));
	m_QuitAction->setStatusTip(tr("Quit the application"));
	connect(m_QuitAction, SIGNAL(triggered()), this, SLOT(close()));
}
void MainWindow::createMenus()
{
	m_FileMenu = menuBar()->addMenu(tr("&File"));
	m_FileMenu->addAction(m_QuitAction);
	m_ViewMenu = menuBar()->addMenu(tr("&View"));
}


void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}
