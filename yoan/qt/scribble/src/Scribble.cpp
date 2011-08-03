
#include <iostream>
#include <QtGui>
#include "Scribble.h"

using namespace std;

Scribble::Scribble()
{
	image = new QImage(300, 300, QImage::Format_RGB32);
	image->fill(qRgb(100, 100, 100));
	penColor = Qt::black;
	penWidth = 3;
	scribbling = false;
}

Scribble::~Scribble()
{
	delete image;
}

QSize Scribble::sizeHint() const
{
	return QSize(320, 320);
}

void Scribble::drawLineTo(const QPoint &endPoint)
{
	QPainter painter(image);
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);
	int rad = (penWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}

void Scribble::mousePressEvent(QMouseEvent* event)
{
	cout << "pressed (" << event->pos().x() << "," << event->pos().y() << ")\n";
	if (event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		this->scribbling = true;
	}

}

void Scribble::mouseMoveEvent(QMouseEvent* event)
{
	cout << "moved (" << event->pos().x() << "," << event->pos().y() << ")\n";
	if ((event->buttons() == Qt::LeftButton) && scribbling) {
		drawLineTo(event->pos());
	}


}

void Scribble::mouseReleaseEvent(QMouseEvent* event)
{
	cout << "released (" << event->pos().x() << "," << event->pos().y() << ")\n";
	if ((event->buttons() == Qt::LeftButton) && scribbling) {
		drawLineTo(event->pos());
		scribbling = false;
	}
}

void Scribble::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect r = event->rect();
	painter.drawImage(r, *image, r);
}

