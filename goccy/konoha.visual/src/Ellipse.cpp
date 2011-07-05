#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Ellipse_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = new KEllipse();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, e);
	RETURN_(p);
}

KMETHOD Ellipse_setRect(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	KRect *r = RawPtr_to(KRect *, sfp[1]);
	e->setRect(*r->r);
	RETURNvoid_();
}

KMETHOD Ellipse_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	e->setBrush(*c);
	RETURNvoid_();
}

KMETHOD Ellipse_setZValue(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	int val = Int_to(int, sfp[1]);
	e->setZValue(val);
	RETURNvoid_();
}

KMETHOD Ellipse_getZValue(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	RETURNi_(e->zValue());
}

#ifdef __cplusplus
}
#endif
