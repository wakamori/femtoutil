#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Timer_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int interval = Int_to(int, sfp[1]);
	knh_Func_t *fo = (knh_Func_t *)sfp[2].fo;
	KTimer *t = new KTimer(interval, fo);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD Timer_start(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTimer *t = RawPtr_to(KTimer *, sfp[0]);
	int timer_id = t->start(ctx, sfp);
	RETURNi_(timer_id);
}

#ifdef __cplusplus
}
#endif
