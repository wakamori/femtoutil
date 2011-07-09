#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Shadow_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsDropShadowEffect *se = new QGraphicsDropShadowEffect();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, se);
	RETURN_(p);
}


#ifdef __cplusplus
}
#endif
