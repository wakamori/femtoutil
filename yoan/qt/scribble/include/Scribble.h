#ifndef SCRIBBLE_H
#define SCRIBBLE_H

#include <QPainter>
#include <QtGui>
#include <QPoint>
#include <QSize>
#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPaintEvent>
#include <QMouseEvent>

class Scribble : public QWidget
{
	Q_OBJECT;

public:
	Scribble();
	~Scribble();
	//void clear();
	QSize sizeHint() const;
	void drawLineTo(const QPoint &endPoint);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void paintEvent(QPaintEvent *event);

private:

	QImage *image;
	QPoint lastPoint;
	bool scribbling;
	int penWidth;
	QColor penColor;
};

#endif /* SCRIBBLE_H */
