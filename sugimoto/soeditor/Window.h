#ifndef CustomMainWindow_H
#define CustomMainWindow_H
#include <QMainWindow>
#include <QtGui>

class QAction;
class QListWidget;
class QMenu;
class Widget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * iParent = 0, Qt::WindowFlags iFlags = 0);

	private:
		void createActions();
		QAction *m_QuitAction;
		QAction *m_TabAction;

		void createMenus();
		QMenu *m_FileMenu;
		QMenu *m_ViewMenu;

		void createStatusBar();

		Widget *m_Widget;
};
#endif 

