#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Brush_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString s = String_to(QString, sfp[1]);
	QColor *c = new QColor(s);
	QBrush *b = new QBrush();
	b->setColor(*c);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, b);
	RETURN_(p);
}

#ifdef __cplusplus
}
#endif
