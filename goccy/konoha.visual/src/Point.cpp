#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Point_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	KPoint *point = new KPoint(x, y);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, point);
	RETURN_(p);
}

KMETHOD Point_getX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KPoint *p = RawPtr_to(KPoint *, sfp[0]);
	RETURNi_(p->x);
}

KMETHOD Point_getY(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KPoint *p = RawPtr_to(KPoint *, sfp[0]);
	RETURNi_(p->y);
}

#ifdef __cplusplus
}
#endif
