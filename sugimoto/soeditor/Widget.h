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
class QListWidget;

class Widget : public QWidget
{
	Q_OBJECT
	public:
		Widget(QWidget * iParent = 0, Qt::WindowFlags iFlags = 0);

	protected slots :
		void newTab();
		void closeTab(int index);
		//void textChecker();
	private:
		void makeWidgets();
		int countTab;

		QHBoxLayout *m_h_StatusLayout;
		QHBoxLayout *m_c_TextLayout;
		QVBoxLayout *m_v_WholeLayout;

		QLabel *m_FileLabel;

		QPushButton *m_FileOpenButton;
		QPushButton *m_AddTabButton;

		QTabWidget *m_Tab;
		QTextEdit *m_TextField;

		QLineEdit *m_FileLineEdit;

		QAction *m_AddTabAction;
};

#endif 
