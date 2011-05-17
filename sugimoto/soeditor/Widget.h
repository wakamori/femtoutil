#ifndef Widget_H
#define Widget_H
#include <QWidget>
#include <QtGui>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QTabWidget;
class QAction;
//int countTab;
class Widget : public QWidget {
	Q_OBJECT

	public:
		Widget(QWidget * iParent = 0, Qt::WindowFlags iFlags = 0);
	public slots :
		void newTab(int countTab);
	private:
		void makeWidgets();
		void newTextField();
		void closeTab();
		void createAddTabAction();
		
		QHBoxLayout *m_h_StatusLayout;
		QHBoxLayout *m_c_TextLayout;
		QVBoxLayout *m_v_WholeLayout;

		QLabel *m_FileLabel;

		QPushButton *m_FileOpenButton;

		QTabWidget *m_Tab;
		QTextEdit *m_TextField;

		QLineEdit *m_FileLineEdit;

		QAction *m_AddTabAction;
};

#endif 
