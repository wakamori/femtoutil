#ifndef CustomMainWindow_H
#define CustomMainWindow_H
#include <QMainWindow>
#include <QtGui>
class MainWindow : public QMainWindow // (1) 
{ Q_OBJECT // (2) 

	public:
		MainWindow(QWidget * iParent = 0, Qt::WindowFlags iFlags = 0);
};
#endif 
