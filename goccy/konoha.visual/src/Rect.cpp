#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

void KGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMousePressEvent(event);
}

void KGraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseReleaseEvent(event);
}

void KGraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseMoveEvent(event);
}

void KGraphicsRectItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	emit emitDragEnterEvent(event);
}

qreal r_prev_x = 0.0f;
qreal r_prev_y = 0.0f;
void KRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KRect::mousePressEvent\n");
	QPointF spos = event->lastScreenPos();
	//fprintf(stderr, "sx: [%f], sy:[%f]\n", spos.x(), spos.y());
	r_prev_x = spos.x();
	r_prev_y = spos.y();
	isDrag = 1;
}

void KRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KRect::mouseMoveEvent\n");
	(void)event;
#ifdef K_USING_BOX2D
	QPointF lspos = event->lastScreenPos();
	if (!isStatic()) {
		setPos(lspos.x() - r_prev_x + x(), lspos.y() - r_prev_y + y());
	}
	r_prev_x = lspos.x();
	r_prev_y = lspos.y();
	isDrag = 1;
#endif
}

void KRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KRect::mouseMoveRelease\n");
	(void)event;
	isDrag = 0;
}

void KRect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KRect::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
}

KRect::KRect(int x, int y, int width, int height)
{
	r = new QRect(x, y, width, height);
	gr = new KGraphicsRectItem();
	gr->setRect(*r);
	connect(gr, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)), this, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)), this, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)), this, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitDragEnterEvent(QGraphicsSceneDragDropEvent*)), this, SLOT(dragEnterEvent(QGraphicsSceneDragDropEvent*)));
	setObjectName("KRect");
#ifdef K_USING_BOX2D
	shapeDef = new b2PolygonDef();
#endif
}

KMETHOD Rect_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	int width = Int_to(int, sfp[3]);
	int height = Int_to(int, sfp[4]);
	//fprintf(stderr, "x = [%d], y = [%d], width = [%d], height = [%d]\n", x, y, width, height);
	KRect *r = new KRect(x, y, width, height);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, r);
	RETURN_(p);
}

KMETHOD Rect_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsRectItem *r = (QGraphicsRectItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	r->setBrush(*c);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D
KMETHOD Rect_setRotation(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	r->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Rect_setDensity(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	r->setDensity(density);
	RETURNvoid_();
}

KMETHOD Rect_setFriction(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	r->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Rect_setRestitution(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	r->setRestitution(restitution);
	RETURNvoid_();
}
#endif

#ifdef __cplusplus
}
#endif
