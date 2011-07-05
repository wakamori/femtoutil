#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD View_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = RawPtr_to(QGraphicsScene *, sfp[1]);
	QGraphicsView *v = new QGraphicsView(s);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, v);
	RETURN_(p);
}

KMETHOD View_setViewportUpdateMode(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsView *v = RawPtr_to(QGraphicsView *, sfp[0]);
	QGraphicsView::ViewportUpdateMode mode = Int_to(QGraphicsView::ViewportUpdateMode, sfp[1]);
	v->setViewportUpdateMode(mode);
	RETURNvoid_();
}

KMETHOD View_setRenderHint(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsView *v = RawPtr_to(QGraphicsView *, sfp[0]);
	QPainter::RenderHint hint = Int_to(QPainter::RenderHint, sfp[1]);
	v->setRenderHint(hint);
	RETURNvoid_();
}

KMETHOD View_show(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsView *v = RawPtr_to(QGraphicsView *, sfp[0]);
	v->show();
	RETURNvoid_();
}

DEFAPI(void) defView(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "View";
}

static knh_IntData_t ViewConstInt[] = {
	{"FullViewportUpdate", QGraphicsView::FullViewportUpdate},
	{NULL, 0}
};

DEFAPI(void) constView(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, ViewConstInt);
}

#ifdef __cplusplus
}
#endif
