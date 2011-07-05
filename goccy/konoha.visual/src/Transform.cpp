#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Transform_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = new QTransform();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD Transform_translate(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = RawPtr_to(QTransform *, sfp[0]);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	t->translate(x, y);
	RETURNvoid_();
}

KMETHOD Transform_rotate(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = RawPtr_to(QTransform *, sfp[0]);
	int angle = Int_to(int, sfp[1]);
	Qt::Axis axis = Int_to(Qt::Axis, sfp[2]);
	t->rotate(angle, axis);
	RETURNvoid_();
}

#ifdef __cplusplus
}
#endif
