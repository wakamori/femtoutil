#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Color_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString s = String_to(QString, sfp[1]);
	QColor *c = new QColor(s);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, c);
	RETURN_(p);
}

#ifdef __cplusplus
}
#endif
