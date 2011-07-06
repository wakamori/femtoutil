#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Group_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGroup *g = new KGroup();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, g);
	RETURN_(p);
}

KMETHOD Group_addToGroup(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGroup *g = RawPtr_to(KGroup *, sfp[0]);
	QGraphicsItem *i = KITEM_to(sfp[1].p);
	g->addToGroup(i);
	RETURNvoid_();
}

#ifdef __cplusplus
}
#endif
